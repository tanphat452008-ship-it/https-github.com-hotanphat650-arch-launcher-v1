package com.byreytiz.launcher.other;

import android.content.Context;
import android.os.Environment;
import android.util.Log;

import java.io.File;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.security.MessageDigest;
import java.security.NoSuchAlgorithmException;
import java.util.ArrayList;
import java.util.List;
import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;

import okhttp3.Call;
import okhttp3.Callback;
import okhttp3.OkHttpClient;
import okhttp3.Request;
import okhttp3.Response;

public class GameDownloader {

    private static final String TAG = "GameDownloader_OkHttp";
    private static final String SERVER_BASE_URL = "http://192.168.1.3/SARP-MB/SARPCity/";
    private static final String BASE_GAME_PATH = "/bytaylyx/";

    private OkHttpClient client;
    private Context context;

    private DownloadListener listener;
    private List<FileChecker.GameFile> filesToDownload;
    private int currentIndex = 0;
    private long totalBytesToDownload = 0; // Tổng dung lượng cần tải (cố định)
    private long downloadedBytes = 0; // Dung lượng đã tải xong

    private final ExecutorService executor = Executors.newSingleThreadExecutor();
    private Call currentCall;

    public interface DownloadListener {
        void onProgress(String fileName, int currentFile, int totalFiles, int progressPercent,
                        String downloadedSize, String totalSize);
        void onFileComplete(String fileName);
        void onAllComplete();
        void onDownloadError(String error);
    }

    public GameDownloader(Context context) {
        this.client = new OkHttpClient();
        this.context = context;
    }

    public void startDownload(List<FileChecker.GameFile> files, DownloadListener listener) {
        this.listener = listener;
        this.currentIndex = 0;
        this.downloadedBytes = 0;

        // Tạo danh sách file cần tải
        this.filesToDownload = new ArrayList<>();

        // Thêm các file từ danh sách gốc
        if (files != null) {
            this.filesToDownload.addAll(files);
        }

        // Kiểm tra và thêm SAMP/settings.ini nếu thiếu
        checkAndAddSettingsFile();

        if (this.filesToDownload.isEmpty()) {
            if (this.listener != null) {
                this.listener.onAllComplete();
            }
            return;
        }

        // DEBUG: Log chi tiết từng file
        Log.d(TAG, "=== DEBUG FILE LIST ===");
        long debugTotalSize = 0;
        for (int i = 0; i < this.filesToDownload.size(); i++) {
            FileChecker.GameFile file = this.filesToDownload.get(i);
            Log.d(TAG, String.format("File %d: %s - Size: %d bytes (%s)",
                    i + 1, file.path, file.size, formatBytes(file.size)));
            debugTotalSize += file.size;
        }
        Log.d(TAG, "Debug calculated total: " + formatBytes(debugTotalSize));
        Log.d(TAG, "=== END DEBUG ===");

        // Tính tổng dung lượng chỉ của các file có size > 0
        totalBytesToDownload = 0;
        int filesWithSize = 0;
        int filesWithoutSize = 0;

        for (FileChecker.GameFile file : this.filesToDownload) {
            if (file.size > 0) {
                totalBytesToDownload += file.size;
                filesWithSize++;
            } else {
                filesWithoutSize++;
            }
        }

        Log.d(TAG, String.format("Total files: %d | With size: %d | Without size: %d",
                this.filesToDownload.size(), filesWithSize, filesWithoutSize));
        Log.d(TAG, "Initial total size to download: " + formatBytes(totalBytesToDownload));

        executor.execute(this::downloadNext);
    }

    private void checkAndAddSettingsFile() {
        String settingsPath = "SAMP/settings.ini";
        String localSettingsPath = Environment.getExternalStorageDirectory().getAbsolutePath() + BASE_GAME_PATH + settingsPath;
        File settingsFile = new File(localSettingsPath);

        if (!settingsFile.exists()) {
            Log.d(TAG, "SAMP/settings.ini not found, adding to download list");
            FileChecker.GameFile settingsGameFile = new FileChecker.GameFile();
            settingsGameFile.path = settingsPath;
            settingsGameFile.size = 0; // Sẽ được cập nhật khi tải
            settingsGameFile.hash = ""; // Có thể bỏ qua verification nếu không có hash

            this.filesToDownload.add(settingsGameFile);
        } else {
            Log.d(TAG, "SAMP/settings.ini already exists, skipping download");
        }
    }

    private void downloadNext() {
        if (currentIndex >= filesToDownload.size()) {
            if (listener != null) {
                listener.onAllComplete();
            }
            return;
        }

        FileChecker.GameFile gameFile = filesToDownload.get(currentIndex);
        String url = SERVER_BASE_URL + gameFile.path;
        String localPath = Environment.getExternalStorageDirectory().getAbsolutePath() + BASE_GAME_PATH + gameFile.path;
        File localFile = new File(localPath);

        // Kiểm tra và xóa file hoặc thư mục cũ trước khi tải
        if (localFile.exists()) {
            if (localFile.isDirectory()) {
                Log.w(TAG, "Path " + localPath + " is a directory. Deleting it.");
                if (!deleteDirectory(localFile)) {
                    Log.e(TAG, "Failed to delete directory: " + localPath);
                    if (listener != null) {
                        listener.onDownloadError("Không thể xóa thư mục cũ để tải file mới: " + localPath);
                    }
                    return;
                }
            } else {
                Log.d(TAG, "File " + localPath + " already exists. Deleting it to overwrite.");
                localFile.delete();
            }
        }

        File parentDir = localFile.getParentFile();
        if (parentDir != null && !parentDir.exists()) {
            parentDir.mkdirs();
        }

        Request request = new Request.Builder().url(url).build();
        currentCall = client.newCall(request);

        currentCall.enqueue(new Callback() {
            @Override
            public void onFailure(Call call, IOException e) {
                Log.e(TAG, "Download failed for " + gameFile.path, e);
                if (listener != null) {
                    listener.onDownloadError("Lỗi tải file " + gameFile.path + ": " + e.getMessage());
                }
            }

            @Override
            public void onResponse(Call call, Response response) throws IOException {
                if (!response.isSuccessful()) {
                    Log.e(TAG, "Server error: " + response.code() + " for " + gameFile.path);
                    if (listener != null) {
                        listener.onDownloadError("Lỗi máy chủ " + response.code() + " khi tải " + gameFile.path);
                    }
                    return;
                }

                InputStream inputStream = null;
                FileOutputStream outputStream = null;
                try {
                    inputStream = response.body().byteStream();
                    outputStream = new FileOutputStream(localFile);
                    long fileDownloadedBytes = 0;
                    byte[] buffer = new byte[8192];
                    int bytesRead;

                    // Lấy actual file size từ response header nếu có
                    long actualFileSize = gameFile.size;
                    String contentLength = response.header("Content-Length");
                    if (contentLength != null && gameFile.size == 0) {
                        try {
                            actualFileSize = Long.parseLong(contentLength);
                            // Cập nhật totalBytesToDownload một lần duy nhất
                            totalBytesToDownload += actualFileSize;
                            Log.d(TAG, String.format("Updated totalBytesToDownload for %s: +%s = %s",
                                    gameFile.path, formatBytes(actualFileSize), formatBytes(totalBytesToDownload)));
                        } catch (NumberFormatException e) {
                            Log.w(TAG, "Cannot parse Content-Length header: " + contentLength);
                        }
                    }

                    while ((bytesRead = inputStream.read(buffer)) != -1) {
                        outputStream.write(buffer, 0, bytesRead);
                        fileDownloadedBytes += bytesRead;

                        long finalFileDownloadedBytes = fileDownloadedBytes;
                        new android.os.Handler(GameDownloader.this.context.getMainLooper()).post(() -> {
                            long overallDownloaded = downloadedBytes + finalFileDownloadedBytes;
                            // Tính phần trăm dựa trên tổng dung lượng cần tải
                            int progressPercent = totalBytesToDownload > 0 ?
                                    (int) ((overallDownloaded * 100) / totalBytesToDownload) : 0;

                            // Đảm bảo progress không vượt quá 100%
                            progressPercent = Math.min(progressPercent, 100);

                            if (listener != null) {
                                listener.onProgress(
                                        gameFile.path,
                                        currentIndex + 1,
                                        filesToDownload.size(),
                                        progressPercent,
                                        formatBytes(overallDownloaded),
                                        formatBytes(totalBytesToDownload)
                                );
                            }
                        });
                    }

                    // Cập nhật size của file nếu chưa có
                    if (gameFile.size == 0) {
                        gameFile.size = localFile.length();
                    }

                    // Kiểm tra verification
                    boolean needVerification = gameFile.hash != null && !gameFile.hash.isEmpty() && gameFile.size > 0;

                    if (needVerification && !verifyFile(localFile, gameFile)) {
                        String errorMsg = "Verification failed for " + gameFile.path +
                                ". Expected size: " + gameFile.size +
                                ", Actual size: " + localFile.length();
                        Log.e(TAG, errorMsg);
                        if (listener != null) {
                            listener.onDownloadError(errorMsg);
                        }
                        localFile.delete();
                        currentIndex++;
                        downloadNext();
                        return;
                    }

                    // Cập nhật downloadedBytes với size thực tế của file
                    downloadedBytes += localFile.length();

                    Log.d(TAG, "File downloaded successfully: " + gameFile.path +
                            " (" + formatBytes(localFile.length()) + ")" +
                            (needVerification ? " (verified)" : " (no verification)"));

                    if (listener != null) {
                        listener.onFileComplete(gameFile.path);
                    }

                    currentIndex++;
                    downloadNext();

                } catch (IOException e) {
                    Log.e(TAG, "IO Exception during download for " + gameFile.path, e);
                    if (listener != null) {
                        listener.onDownloadError("Lỗi đọc/ghi file khi tải " + gameFile.path + ": " + e.getMessage());
                    }
                } finally {
                    if (inputStream != null) {
                        inputStream.close();
                    }
                    if (outputStream != null) {
                        outputStream.close();
                    }
                    response.close();
                }
            }
        });
    }

    private boolean verifyFile(File file, FileChecker.GameFile expectedFile) {
        if (!file.exists()) {
            return false;
        }

        if (file.length() != expectedFile.size) {
            Log.e(TAG, "Size mismatch: " + file.length() + " vs " + expectedFile.size);
            return false;
        }

        String calculatedHash = getFileHash(file);
        if (calculatedHash == null || !calculatedHash.equalsIgnoreCase(expectedFile.hash)) {
            Log.e(TAG, "Hash mismatch: " + calculatedHash + " vs " + expectedFile.hash);
            return false;
        }

        Log.d(TAG, "File verified successfully: " + file.getName());
        return true;
    }

    private String getFileHash(File file) {
        try {
            MessageDigest digest = MessageDigest.getInstance("MD5");
            InputStream is = new java.io.FileInputStream(file);
            byte[] buffer = new byte[8192];
            int read;
            while ((read = is.read(buffer)) > 0) {
                digest.update(buffer, 0, read);
            }
            is.close();
            byte[] md5sum = digest.digest();
            StringBuilder hexString = new StringBuilder();
            for (byte b : md5sum) {
                hexString.append(String.format("%02x", b & 0xff));
            }
            return hexString.toString();
        } catch (IOException | NoSuchAlgorithmException e) {
            Log.e(TAG, "Failed to calculate file hash", e);
            return null;
        }
    }

    public boolean deleteDirectory(File dir) {
        if (dir.isDirectory()) {
            String[] children = dir.list();
            for (int i = 0; i < children.length; i++) {
                boolean success = deleteDirectory(new File(dir, children[i]));
                if (!success) {
                    return false;
                }
            }
        }
        return dir.delete();
    }

    public void cancel() {
        if (currentCall != null) {
            currentCall.cancel();
        }
        executor.shutdownNow();
        Log.d(TAG, "All download tasks cancelled.");
    }

    private String formatBytes(long bytes) {
        if (bytes < 1024) return bytes + " B";
        int exp = (int) (Math.log(bytes) / Math.log(1024));
        String pre = "KMGTPE".charAt(exp - 1) + "";
        return String.format("%.1f %sB", bytes / Math.pow(1024, exp), pre);
    }
}