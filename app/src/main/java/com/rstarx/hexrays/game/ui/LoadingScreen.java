package com.rstarx.hexrays.game.ui;

import android.app.Activity;
import android.view.View;
import android.widget.TextView;
import androidx.constraintlayout.widget.ConstraintLayout;
import com.rstarx.hexrays.R;
import java.io.File;

public class LoadingScreen {

    private Activity activity;
    private ConstraintLayout mainLayout;
    private TextView statusText;

    public LoadingScreen(Activity activity) {
        this.activity = activity;

        // Inflate layout และหา TextView
        mainLayout = (ConstraintLayout) activity.getLayoutInflater().inflate(R.layout.loadingscreen, null);
        statusText = mainLayout.findViewById(R.id.loadingStatusText);

        // เพิ่มเข้าหน้าจอ และตั้งค่าเริ่มต้นเป็น INVISIBLE หรือ GONE ก่อนก็ได้
        activity.addContentView(mainLayout, new ConstraintLayout.LayoutParams(-1, -1));
        mainLayout.setVisibility(View.GONE);
    }

    // --- เพิ่ม Method นี้เพื่อแก้ Error line 42 ---
    public void show() {
        activity.runOnUiThread(() -> {
            mainLayout.setVisibility(View.VISIBLE);
        });
    }

    // --- เพิ่ม Method นี้เพื่อแก้ Error line 43 ---
    public void startLoadingProcess() {
        new Thread(() -> {
            File root = activity.getExternalFilesDir(null);
            if (root == null) return;

            // สแกนโฟลเดอร์ data/
            scanFolder(new File(root, "data"), "data/");

            // สแกนโฟลเดอร์ texdb/
            scanFolder(new File(root, "texdb"), "texdb/");

            // เมื่อเสร็จแล้ว (Option: จะสั่ง hide เลย หรือรอ Native สั่งก็ได้)
            // updateStatus("โหลดเสร็จสมบูรณ์");
        }).start();
    }

    private void scanFolder(File folder, String prefix) {
        if (folder.exists() && folder.isDirectory()) {
            File[] files = folder.listFiles();
            if (files != null) {
                for (File f : files) {
                    if (f.isFile()) {
                        updateStatus(prefix + f.getName());
                        // หน่วงเวลาเล็กน้อยให้เห็นชื่อไฟล์วิ่ง
                        try { Thread.sleep(50); } catch (InterruptedException ignored) {}
                    }
                }
            }
        }
    }

    public void updateStatus(final String fileName) {
        activity.runOnUiThread(() -> {
            if (statusText != null) {
                statusText.setText("กำลังโหลด : " + fileName);
            }
        });
    }

    public void hide() {
        activity.runOnUiThread(() -> {
            mainLayout.setVisibility(View.GONE);
        });
    }
}