package com.byreytiz.launcher.other;

import android.content.Context;
import android.os.Environment;
import android.util.Log;

import com.google.gson.Gson;
import com.google.gson.JsonObject;
import com.google.gson.JsonElement;

import java.io.File;
import java.io.IOException;
import java.util.ArrayList;
import java.util.List;
import java.util.Map;

import okhttp3.Call;
import okhttp3.Callback;
import okhttp3.OkHttpClient;
import okhttp3.Request;
import okhttp3.Response;

public class FileChecker {
    private static final String TAG = "FileChecker";
    private static final String SERVER_FILES_JSON_URL = "http://192.168.1.3/SARP-MB/files.json";
    private static final String BASE_GAME_PATH = "/bytaylyx/";

    private Context context;
    private CheckListener listener;
    private OkHttpClient httpClient;

    public interface CheckListener {
        void onNeedUpdate(List<GameFile> filesToUpdate);
        void onUpToDate();
        void onError(String error);
    }

    public static class GameFile {
        public String path;
        public String hash;
        public long size;
        public double mtime;

        public GameFile(String path, String hash, long size, double mtime) {
            this.path = path;
            this.hash = hash;
            this.size = size;
            this.mtime = mtime;
        }

        // Constructor không có tham số cho settings.ini
        public GameFile() {
        }
    }

    public FileChecker(Context context) {
        this.context = context;
        this.httpClient = new OkHttpClient();
    }

    public void checkFiles(CheckListener listener) {
        this.listener = listener;

        Request request = new Request.Builder()
                .url(SERVER_FILES_JSON_URL)
                .build();

        httpClient.newCall(request).enqueue(new Callback() {
            @Override
            public void onFailure(Call call, IOException e) {
                Log.e(TAG, "Failed to fetch server files", e);
                if (listener != null) {
                    listener.onError("Không thể kết nối server: " + e.getMessage());
                }
            }

            @Override
            public void onResponse(Call call, Response response) throws IOException {
                if (!response.isSuccessful()) {
                    if (listener != null) {
                        listener.onError("Server error: " + response.code());
                    }
                    return;
                }

                String jsonString = response.body().string();
                processServerFiles(jsonString);
            }
        });
    }

    private void processServerFiles(String jsonString) {
        try {
            Gson gson = new Gson();
            JsonObject jsonObject = gson.fromJson(jsonString, JsonObject.class);
            JsonObject filesObject = jsonObject.getAsJsonObject("files");

            List<GameFile> filesToUpdate = new ArrayList<>();

            // Kiểm tra các file từ server
            for (Map.Entry<String, JsonElement> entry : filesObject.entrySet()) {
                String filePath = entry.getKey();
                JsonObject fileData = entry.getValue().getAsJsonObject();

                String serverHash = fileData.get("hash").getAsString();
                long serverSize = fileData.get("size").getAsLong();
                double serverMtime = fileData.get("mtime").getAsDouble();

                GameFile gameFile = new GameFile(filePath, serverHash, serverSize, serverMtime);

                if (needsUpdate(filePath, serverSize)) {
                    filesToUpdate.add(gameFile);
                }
            }

            // Ghi log để kiểm tra
            long totalUpdateSize = 0;
            Log.d(TAG, "=== Files need to update ===");
            for (GameFile file : filesToUpdate) {
                totalUpdateSize += file.size;
                Log.d(TAG, "File: " + file.path + ", Size: " + file.size + ", Hash: " + file.hash);
            }
            Log.d(TAG, "========================");
            Log.d(TAG, "Files to update: " + filesToUpdate.size() + ", Total size: " + totalUpdateSize);

            new android.os.Handler(context.getMainLooper()).post(() -> {
                if (filesToUpdate.isEmpty()) {
                    if (listener != null) {
                        listener.onUpToDate();
                    }
                } else {
                    if (listener != null) {
                        listener.onNeedUpdate(filesToUpdate);
                    }
                }
            });

        } catch (Exception e) {
            Log.e(TAG, "Error processing server response", e);
            if (listener != null) {
                listener.onError("Lỗi xử lý dữ liệu: " + e.getMessage());
            }
        }
    }



    private boolean needsUpdate(String filePath, long serverSize) {
        File localFile = new File(Environment.getExternalStorageDirectory() + BASE_GAME_PATH + filePath);

        // Đặc biệt cho SAMP/settings.ini: có file thì bỏ qua luôn
        if ("SAMP/settings.ini".equals(filePath)) {
            if (localFile.exists() && !localFile.isDirectory()) {
                Log.d(TAG, "SAMP/settings.ini đã có, bỏ qua không cần update");
                return false;
            } else {
                Log.d(TAG, "SAMP/settings.ini không có hoặc là thư mục, cần tải");
                return true;
            }
        }

        // Logic bình thường cho các file khác
        if (!localFile.exists()) {
            Log.d(TAG, "File missing: " + filePath);
            return true;
        }

        if (localFile.isDirectory()) {
            Log.d(TAG, "Path is directory, needs update: " + filePath);
            return true;
        }

        if (localFile.length() != serverSize) {
            Log.d(TAG, "Size different for " + filePath + ": local=" + localFile.length() + " vs server=" + serverSize);
            return true;
        }

        Log.d(TAG, "File OK: " + filePath);
        return false;
    }

    public static boolean isGameInstalled() {
        createGameDirectories();
        String mainFile = Environment.getExternalStorageDirectory() + "/bytaylyx/texdb/gta3.img";
        File file = new File(mainFile);
        return file.exists() && file.length() > 0;
    }

    public static void createGameDirectories() {
        String basePath = Environment.getExternalStorageDirectory() + "/bytaylyx/";
        String[] directories = {
                basePath,
                basePath + "texdb/",
                basePath + "SAMP/",
                basePath + "data/",
                basePath + "models/",
                basePath + "audio/"
        };
        for (String dirPath : directories) {
            File dir = new File(dirPath);
            if (!dir.exists()) {
                boolean created = dir.mkdirs();
                Log.d(TAG, "Created directory: " + dirPath + " - " + created);
            }
        }
    }
}