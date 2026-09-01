package com.rstarx.hexrays.launcher;

import android.app.Service;
import android.content.Intent;
import android.os.Bundle;
import android.os.Handler;
import android.os.HandlerThread;
import android.os.IBinder;
import android.os.Message;
import android.os.Messenger;
import android.os.Process;
import android.util.Log;

import com.android.volley.toolbox.StringRequest;
import com.android.volley.toolbox.Volley;
import com.downloader.PRDownloader;
import com.downloader.PRDownloaderConfig;
import com.downloader.Priority;
import com.joom.paranoid.Obfuscate;

import org.json.JSONObject;

import java.io.File;
import java.util.Scanner;

@Obfuscate
public class UpdateService extends Service {
    private Messenger mMessenger;
    private Messenger mActivityMessenger;

    // ตัวแปรสำหรับเก็บ URL ที่จะโหลด (จะเลือกเป็นตัวเต็มหรือตัวอัปเดตตามเงื่อนไข)
    private String mTargetDownloadURL = "";

    @Override
    public void onCreate() {
        super.onCreate();

        PRDownloaderConfig config = PRDownloaderConfig.newBuilder()
                .setDatabaseEnabled(true)
                .setReadTimeout(30_000)
                .setConnectTimeout(30_000)
                .setHttpClient(new com.downloader.httpclient.DefaultHttpClient())
                .build();
        PRDownloader.initialize(this, config);

        HandlerThread thread = new HandlerThread("UpdateHighPriorityThread", Process.THREAD_PRIORITY_FOREGROUND);
        thread.start();

        mMessenger = new Messenger(new Handler(thread.getLooper()) {
            @Override
            public void handleMessage(Message msg) {
                mActivityMessenger = msg.replyTo;
                if (msg.what == 0) {
                    checkUpdate();
                }
                else if (msg.what == 2) {
                    downloadZip();
                }
            }
        });
    }

    private int getLocalDataVersion() {
        File vFile = new File(getExternalFilesDir(null), "SAMP/version.ini");
        if (!vFile.exists()) return 0;
        try (Scanner sc = new Scanner(vFile)) {
            while (sc.hasNextLine()) {
                String line = sc.nextLine();
                if (line.contains("version=")) {
                    return Integer.parseInt(line.split("=")[1].trim());
                }
            }
        } catch (Exception e) {
            Log.e("SAMP_UPDATE", "Read local version error: " + e.getMessage());
        }
        return 0;
    }

    private void checkUpdate() {
        String url = "http://103.216.158.30/test/api.json";

        StringRequest request = new StringRequest(url, response -> {
            try {
                JSONObject jsonObject = new JSONObject(response);
                JSONObject config = jsonObject.getJSONObject("client_config");

                int webVer = config.getInt("data_version_code");
                String fullZipURL = config.getString("latest_game_zip");
                String updateZipURL = config.optString("link_upd", fullZipURL); // ถ้าไม่มี link_upd ให้ใช้ตัวเต็มแทน

                int localVer = getLocalDataVersion();

                if (localVer < webVer) {
                    // --- ส่วนตัดสินใจเลือกไฟล์ ---
                    if (localVer == 0) {
                        // ไม่เคยมีไฟล์เลย หรือไฟล์ version.ini หาย ให้โหลดตัวเต็ม
                        mTargetDownloadURL = fullZipURL;
                        Log.d("SAMP_UPDATE", "Action: Full Download");
                    } else {
                        // มีเวอร์ชันเก่าอยู่แล้ว ให้โหลดเฉพาะไฟล์อัปเดต
                        mTargetDownloadURL = updateZipURL;
                        Log.d("SAMP_UPDATE", "Action: Patch Update");
                    }

                    sendToActivity(5, "UpdateRequired", null);
                } else {
                    sendToActivity(5, "Updated", null);
                }
            } catch (Exception e) {
                sendToActivity(5, "Unknown", null);
            }
        }, error -> sendToActivity(5, "Unknown", null));

        Volley.newRequestQueue(this).add(request);
    }

    private void downloadZip() {
        if (mTargetDownloadURL == null || mTargetDownloadURL.isEmpty()) {
            Log.e("SAMP_UPDATE", "Download URL is empty");
            return;
        }

        String path = getExternalFilesDir(null) + "/download";
        File dir = new File(path);
        if (!dir.exists()) dir.mkdirs();

        File file = new File(path, "update.zip");
        if (file.exists()) file.delete();

        PRDownloader.download(mTargetDownloadURL, path, "update.zip")
                .setPriority(Priority.IMMEDIATE)
                .build()
                .setOnProgressListener(p -> {
                    sendProgress(p.currentBytes, p.totalBytes);
                })
                .start(new com.downloader.OnDownloadListener() {
                    @Override
                    public void onDownloadComplete() {
                        sendToActivity(2, "true", file.getAbsolutePath());
                    }

                    @Override
                    public void onError(com.downloader.Error error) {
                        Log.e("SAMP_UPDATE", "Download failed: " + error.getServerErrorMessage());
                    }
                });
    }

    private void sendProgress(long cur, long total) {
        Message msg = Message.obtain(null, 4);
        Bundle b = new Bundle();
        b.putLong("current", cur);
        b.putLong("total", total);
        b.putString("status", "DownloadGame");
        b.putString("filename", "กำลังดาวน์โหลดไฟล์ข้อมูล...");
        msg.setData(b);
        try {
            if (mActivityMessenger != null) mActivityMessenger.send(msg);
        } catch (Exception e) {
            e.printStackTrace();
        }
    }

    private void sendToActivity(int what, String status, String path) {
        Message msg = Message.obtain(null, what);
        Bundle b = new Bundle();
        b.putString("status", status);
        if (path != null) b.putString("apkPath", path);
        msg.setData(b);
        try {
            if (mActivityMessenger != null) mActivityMessenger.send(msg);
        } catch (Exception e) {
            e.printStackTrace();
        }
    }

    @Override
    public IBinder onBind(Intent intent) {
        return mMessenger.getBinder();
    }

    @Override
    public void onDestroy() {
        PRDownloader.shutDown();
        super.onDestroy();
    }
}