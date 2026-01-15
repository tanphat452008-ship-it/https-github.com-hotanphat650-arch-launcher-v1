package com.byreytiz.launcher.activity;

import android.content.Intent;
import android.os.Bundle;
import android.os.Environment;
import android.view.WindowManager;
import android.widget.TextView;
import android.widget.Toast;

import androidx.appcompat.app.AppCompatActivity;

import com.akexorcist.roundcornerprogressbar.RoundCornerProgressBar;
import com.byreytiz.game.R;
import com.byreytiz.launcher.other.FileChecker;
import com.byreytiz.launcher.other.GameDownloader;

import com.hzy.libp7zip.P7ZipApi;
import com.liulishuo.filedownloader.BaseDownloadTask;
import com.liulishuo.filedownloader.FileDownloadSampleListener;
import com.liulishuo.filedownloader.FileDownloader;

import java.io.File;
import java.util.List;

public class LoaderActivity extends AppCompatActivity implements
        FileChecker.CheckListener, GameDownloader.DownloadListener {

    RoundCornerProgressBar progressbar;
    TextView textprogress;
    TextView textloading;

    private FileChecker fileChecker;
    private GameDownloader gameDownloader;
    private boolean isDownloadingFiles = false;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.fragment_loader);

        FileDownloader.setup(this);
        getWindow().addFlags(WindowManager.LayoutParams.FLAG_KEEP_SCREEN_ON);

        // Init views
        progressbar = findViewById(R.id.progress_bar);
        textprogress = findViewById(R.id.loading_percent);
        textloading = findViewById(R.id.loading_text);

        // Init downloaders
        fileChecker = new FileChecker(this);
        gameDownloader = new GameDownloader(this);

        // Start checking
        startCheck();
    }

    private void startCheck() {
        textloading.setText("Đang kiểm tra files...");
        textprogress.setText("0%");
        progressbar.setProgress(0);

        fileChecker.checkFiles(this);
    }

    // FileChecker.CheckListener
    @Override
    public void onNeedUpdate(List<FileChecker.GameFile> filesToUpdate) {
        runOnUiThread(() -> {
            textloading.setText("Bắt đầu tải " + filesToUpdate.size() + " file(s)...");
            isDownloadingFiles = true;
            gameDownloader.startDownload(filesToUpdate, this);
        });
    }

    @Override
    public void onUpToDate() {
        runOnUiThread(() -> {
            if (!FileChecker.isGameInstalled()) {
                // Need to download main game
                textloading.setText("Tải game chính...");
                downloadMainGame();
            } else {
                // All done
                finishInstall();
            }
        });
    }

    @Override
    public void onError(String error) {
        runOnUiThread(() -> {
            textloading.setText("Lỗi kiểm tra files");
            textprogress.setText(error);
            Toast.makeText(this, "Lỗi: " + error, Toast.LENGTH_LONG).show();

            // Fallback to main game download
            downloadMainGame();
        });
    }

    // GameDownloader.DownloadListener
    @Override
    public void onProgress(String fileName, int currentFile, int totalFiles, int progressPercent,
                           String downloadedSize, String totalSize) {
        runOnUiThread(() -> {
            progressbar.setProgress(progressPercent);
            textloading.setText("Tải: " + getFileName(fileName) + " (" + currentFile + "/" + totalFiles + ")");
            textprogress.setText(downloadedSize + " / " + totalSize + " (" + progressPercent + "%)");
        });
    }

    @Override
    public void onFileComplete(String fileName) { }

    @Override
    public void onAllComplete() {
        runOnUiThread(() -> {
            textloading.setText("Tải thành công!");
            textprogress.setText("100%");
            progressbar.setProgress(100);

            // Wait a bit then finish
            progressbar.postDelayed(this::finishInstall, 1000);
        });
    }

    public void onDownloadError(String error) {
        runOnUiThread(() -> {
            textloading.setText("Lỗi tải file");
            textprogress.setText(error);
            Toast.makeText(this, "Lỗi: " + error, Toast.LENGTH_LONG).show();

            // Fallback after 3 seconds
            /* progressbar.postDelayed(() -> {
                Toast.makeText(this, "Chuyển sang tải dự phòng...", Toast.LENGTH_SHORT).show();
                downloadMainGame();
            }, 3000); */
        });
    }

    // Legacy download main game archive
    private void downloadMainGame() {
        isDownloadingFiles = false;

        File folder = Environment.getExternalStoragePublicDirectory(Environment.DIRECTORY_DOWNLOADS);
        String url = "https://www.dropbox.com/scl/fi/90n7u6ea346u06rrxjmzr/black-cache.7z?rlkey=b07fm2ob8bttb9camgs2qfbg6&st=xy9ut3fj&dl=1";

        BaseDownloadTask task = FileDownloader.getImpl()
                .create(url)
                .setPath(folder.getPath(), true)
                .setCallbackProgressTimes(100)
                .setMinIntervalUpdateSpeed(100)
                .setListener(new FileDownloadSampleListener() {

                    @Override
                    protected void progress(BaseDownloadTask task, int soFarBytes, int totalBytes) {
                        long progressPercent = soFarBytes * 100L / totalBytes;

                        runOnUiThread(() -> {
                            textloading.setText("Đang tải game chính...");
                            textprogress.setText(formatBytes(soFarBytes) + " / " + formatBytes(totalBytes));
                            progressbar.setProgress((int) progressPercent);
                        });
                    }

                    @Override
                    protected void error(BaseDownloadTask task, Throwable e) {
                        runOnUiThread(() -> {
                            Toast.makeText(LoaderActivity.this,
                                    "Lỗi tải game. Vui lòng thử lại!", Toast.LENGTH_SHORT).show();
                            startActivity(new Intent(LoaderActivity.this, MainActivity.class));
                            finish();
                        });
                    }

                    @Override
                    protected void completed(BaseDownloadTask task) {
                        runOnUiThread(() -> {
                            textloading.setText("Đang giải nén...");
                            textprogress.setText("Vui lòng đợi...");
                            extractMainGame();
                        });
                    }
                });

        task.start();
    }

    private void extractMainGame() {
        String inputFile = Environment.getExternalStoragePublicDirectory(Environment.DIRECTORY_DOWNLOADS) + "/black-cache.7z";
        String outputPath = Environment.getExternalStorageDirectory().toString();

        new Thread(() -> {
            try {
                P7ZipApi.executeCommand(String.format("7z x '%s' '-o%s' -aoa", inputFile, outputPath));

                // Clean up
                deleteFile(new File(inputFile));
                deleteFile(new File(inputFile + ".temp"));

                runOnUiThread(this::finishInstall);

            } catch (Exception e) {
                runOnUiThread(() -> {
                    Toast.makeText(this, "Lỗi giải nén: " + e.getMessage(), Toast.LENGTH_LONG).show();
                    startActivity(new Intent(this, MainActivity.class));
                    finish();
                });
            }
        }).start();
    }

    private void finishInstall() {
        Toast.makeText(this, "Cài đặt thành công!", Toast.LENGTH_SHORT).show();
        startActivity(new Intent(this, MainActivity.class));
        finish();
    }

    private String getFileName(String fullPath) {
        if (fullPath == null || fullPath.isEmpty()) return "";
        int lastSlash = fullPath.lastIndexOf('/');
        return lastSlash >= 0 ? fullPath.substring(lastSlash + 1) : fullPath;
    }

    private String formatBytes(long bytes) {
        if (bytes < 1024) return bytes + " B";
        int exp = (int) (Math.log(bytes) / Math.log(1024));
        String pre = "KMGTPE".charAt(exp-1) + "";
        return String.format("%.1f %sB", bytes / Math.pow(1024, exp), pre);
    }

    private void deleteFile(File file) {
        if (file != null && file.exists()) {
            file.delete();
        }
    }

    @Override
    protected void onDestroy() {
        super.onDestroy();
        if (isDownloadingFiles && gameDownloader != null) {
            gameDownloader.cancel();
        }
    }

    @Override
    public void onBackPressed() {
        if (isDownloadingFiles) {
            Toast.makeText(this, "Đang tải, vui lòng đợi...", Toast.LENGTH_SHORT).show();
            return;
        }
        super.onBackPressed();
    }
}