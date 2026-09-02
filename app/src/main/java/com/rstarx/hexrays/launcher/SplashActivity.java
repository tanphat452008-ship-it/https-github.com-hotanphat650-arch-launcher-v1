package com.rstarx.hexrays.launcher;

import android.content.Intent;
import android.content.pm.PackageManager;
import android.net.Uri;
import android.os.Build;
import android.os.Bundle;
import android.os.Environment;
import android.os.Handler;
import android.os.Looper;
import android.provider.Settings;
import android.util.Log;
import android.widget.Toast;

import androidx.annotation.NonNull;
import androidx.appcompat.app.AlertDialog;
import androidx.appcompat.app.AppCompatActivity;
import androidx.core.app.ActivityCompat;
import androidx.core.content.ContextCompat;

import com.rstarx.hexrays.R;
import com.rstarx.hexrays.launcher.config.Config;

public class SplashActivity extends AppCompatActivity {
    private static final String TAG = "SplashActivity";
    private static final int ALL_FILES_REQUEST_CODE = 2222;
    private static final int BASIC_PERMISSION_CODE = 1111;

    private final String[] basicPermissions = {
            "android.permission.RECORD_AUDIO"
    };

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_splash);

        Config.currentContext = this;

        // Bỏ qua Check Signature & Check GPU -> Bắt đầu xin quyền luôn
        startPermissionSequence();
    }

    private void startPermissionSequence() {
        if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.R) {
            if (Environment.isExternalStorageManager()) {
                checkBasicPermissions();
            } else {
                showAllFilesDialog();
            }
        } else {
            checkBasicPermissions();
        }
    }

    private void showAllFilesDialog() {
        new AlertDialog.Builder(this)
                .setTitle("การอนุญาตเข้าถึงไฟล์")
                .setMessage("จำเป็นต้องเปิดสิทธิ์ 'เข้าถึงไฟล์ทั้งหมด' เพื่อติดตั้งและโหลดข้อมูลเกม\n\n1. กด 'ตกลง'\n2. หาแอป ในรายการ\n3. กดเปิดสวิตช์อนุญาต")
                .setCancelable(false)
                .setPositiveButton("ตกลง", (dialog, which) -> {
                    try {
                        Intent intent = new Intent(Settings.ACTION_MANAGE_APP_ALL_FILES_ACCESS_PERMISSION);
                        intent.setData(Uri.parse("package:" + getPackageName()));
                        startActivityForResult(intent, ALL_FILES_REQUEST_CODE);
                    } catch (Exception e) {
                        Intent intent = new Intent(Settings.ACTION_MANAGE_ALL_FILES_ACCESS_PERMISSION);
                        startActivityForResult(intent, ALL_FILES_REQUEST_CODE);
                    }
                })
                .setNegativeButton("ออกเกม", (dialog, which) -> finish())
                .show();
    }

    private void checkBasicPermissions() {
        if (ContextCompat.checkSelfPermission(this, "android.permission.RECORD_AUDIO") != PackageManager.PERMISSION_GRANTED) {
            ActivityCompat.requestPermissions(this, basicPermissions, BASIC_PERMISSION_CODE);
        } else {
            goToMainActivity();
        }
    }

    @Override
    protected void onActivityResult(int requestCode, int resultCode, Intent data) {
        super.onActivityResult(requestCode, resultCode, data);
        if (requestCode == ALL_FILES_REQUEST_CODE) {
            new Handler(Looper.getMainLooper()).postDelayed(() -> {
                if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.R) {
                    if (Environment.isExternalStorageManager()) {
                        checkBasicPermissions();
                    } else {
                        Toast.makeText(this, "คุณยังไม่ได้อนุญาตสิทธิ์ไฟล์!", Toast.LENGTH_LONG).show();
                        showAllFilesDialog();
                    }
                }
            }, 1000);
        }
    }

    @Override
    public void onRequestPermissionsResult(int requestCode, @NonNull String[] permissions, @NonNull int[] grantResults) {
        super.onRequestPermissionsResult(requestCode, permissions, grantResults);
        if (requestCode == BASIC_PERMISSION_CODE) {
            goToMainActivity();
        }
    }

    private void goToMainActivity() {
        Intent intent = new Intent(SplashActivity.this, MainActivity.class);
        startActivity(intent);
        finish();
    }
}
