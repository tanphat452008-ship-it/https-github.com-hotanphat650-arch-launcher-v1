package com.rstarx.hexrays.launcher;

import android.content.ComponentName;
import android.content.Context;
import android.content.Intent;
import android.content.ServiceConnection;
import android.os.Bundle;
import android.os.Handler;
import android.os.IBinder;
import android.os.Message;
import android.os.Messenger;
import android.os.RemoteException;
import android.view.WindowManager;
import android.widget.TextView;
import android.widget.Toast;
import androidx.appcompat.app.AppCompatActivity;

// Import ตัว ProgressBar ที่คุณใช้ใน XML
import com.akexorcist.roundcornerprogressbar.RoundCornerProgressBar;
import com.joom.paranoid.Obfuscate;
import com.rstarx.hexrays.R;

import java.io.BufferedInputStream;
import java.io.BufferedOutputStream;
import java.io.File;
import java.io.FileInputStream;
import java.io.FileOutputStream;
import java.io.IOException;
import java.util.zip.ZipEntry;
import java.util.zip.ZipInputStream;

@Obfuscate
public class UpdateActivity extends AppCompatActivity {

    public Messenger mMessenger = new Messenger(new IncomingHandler());
    public Messenger mService;
    private boolean mIsBind;
    private File mDownloadedZip;

    // --- ประกาศตัวแปรให้ตรงกับ ID ใน XML ---
    private TextView file_name;       // ID: file_name
    private TextView percent_name;   // ID: percent_name
    private TextView percent_mem;    // ID: percent_mem
    private RoundCornerProgressBar progressView; // ID: progressView

    private long lastUiUpdateTime = 0;

    class IncomingHandler extends Handler {
        @Override
        public void handleMessage(Message msg) {
            // ID 5: สถานะจาก API Check Update
            if (msg.what == 5) {
                String status = msg.getData().getString("status");
                if ("UpdateRequired".equals(status)) {
                    file_name.setText("สถานะ : กำลังเริ่มดาวน์โหลด...");
                    startDownloadFromService();
                } else if ("Updated".equals(status)) {
                    startActivity(new Intent(UpdateActivity.this, MainActivity.class));
                    finish();
                } else {
                    file_name.setText("สถานะ : ข้อผิดพลาด API");
                }
            }
            // ID 4: รับ Progress จากการดาวน์โหลด
            else if (msg.what == 4) {
                long now = System.currentTimeMillis();
                if (now - lastUiUpdateTime < 150) return;
                lastUiUpdateTime = now;

                long total = msg.getData().getLong("total");
                long current = msg.getData().getLong("current");
                String fileNameText = msg.getData().getString("filename");

                int progressPercent = (total > 0) ? (int) ((current * 100) / total) : 0;

                // --- อัปเดต UI ตาม ID ที่ตรงกับ XML ---
                progressView.setProgress(progressPercent);
                percent_name.setText(progressPercent + "%");
                file_name.setText("โหลดไฟล์ : " + fileNameText);

                double curMB = current / 1048576.0;
                double totMB = total / 1048576.0;
                percent_mem.setText(String.format("%.2f MB / %.2f MB", curMB, totMB));
            }
            // ID 2: ดาวน์โหลดเสร็จสิ้น
            else if (msg.what == 2) {
                String path = msg.getData().getString("apkPath", "");
                mDownloadedZip = new File(path);
                if (mDownloadedZip.exists()) {
                    startUnzipProcess();
                }
            }
        }
    }

    private void startDownloadFromService() {
        try {
            Message msg = Message.obtain(null, 2);
            msg.replyTo = mMessenger;
            mService.send(msg);
        } catch (RemoteException e) { e.printStackTrace(); }
    }

    private void startUnzipProcess() {
        new Thread(() -> {
            try {
                runOnUiThread(() -> {
                    file_name.setText("สถานะ : กำลังแตกไฟล์ข้อมูล...");
                    percent_name.setText("รอ...");
                    progressView.setProgress(100); // ตั้งให้เต็มหลอดระหว่างแตกไฟล์
                });

                unzip(mDownloadedZip, getExternalFilesDir(null));

                if (mDownloadedZip.exists()) mDownloadedZip.delete();

                runOnUiThread(() -> {
                    Toast.makeText(this, "ติดตั้งสำเร็จ!", Toast.LENGTH_SHORT).show();
                    startActivity(new Intent(this, MainActivity.class));
                    finish();
                });
            } catch (IOException e) {
                runOnUiThread(() -> Toast.makeText(this, "แตกไฟล์ผิดพลาด: " + e.getMessage(), Toast.LENGTH_LONG).show());
            }
        }).start();
    }

    private void unzip(File zipFile, File targetDir) throws IOException {
        int BUFFER_SIZE = 1024 * 256;
        try (ZipInputStream zis = new ZipInputStream(new BufferedInputStream(new FileInputStream(zipFile), BUFFER_SIZE))) {
            ZipEntry ze;
            byte[] buffer = new byte[BUFFER_SIZE];
            while ((ze = zis.getNextEntry()) != null) {
                File file = new File(targetDir, ze.getName());
                File dir = ze.isDirectory() ? file : file.getParentFile();
                if (!dir.exists() && !dir.mkdirs()) throw new IOException("Failed to create dir: " + dir);
                if (ze.isDirectory()) continue;

                try (BufferedOutputStream bos = new BufferedOutputStream(new FileOutputStream(file), BUFFER_SIZE)) {
                    int count;
                    while ((count = zis.read(buffer)) != -1) {
                        bos.write(buffer, 0, count);
                    }
                    bos.flush();
                }
                zis.closeEntry();
            }
        }
    }

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_update);

        // --- ผูกตัวแปร Java เข้ากับ ID ใน XML (ตรงกันทุกตัว) ---
        file_name = findViewById(R.id.file_name);
        percent_name = findViewById(R.id.percent_name);
        percent_mem = findViewById(R.id.percent_mem);
        progressView = findViewById(R.id.progressView);

        getWindow().addFlags(WindowManager.LayoutParams.FLAG_KEEP_SCREEN_ON);

        bindService(new Intent(this, UpdateService.class), serviceConnection, BIND_AUTO_CREATE);
    }

    private ServiceConnection serviceConnection = new ServiceConnection() {
        @Override
        public void onServiceConnected(ComponentName name, IBinder binder) {
            mService = new Messenger(binder);
            mIsBind = true;
            try {
                Message msg = Message.obtain(null, 0);
                msg.replyTo = mMessenger;
                mService.send(msg);
            } catch (RemoteException e) { e.printStackTrace(); }
        }
        @Override public void onServiceDisconnected(ComponentName name) { mIsBind = false; }
    };

    @Override
    protected void onDestroy() {
        super.onDestroy();
        if(mIsBind) unbindService(serviceConnection);
    }
}