package com.rstarx.hexrays.launcher;

import android.content.*;
import android.content.pm.PackageManager;
import android.net.Uri;
import android.opengl.GLSurfaceView;
import android.os.*;
import android.provider.Settings;
import android.util.Log;
import android.widget.TextView;
import android.widget.Toast;

import androidx.annotation.NonNull;
import androidx.appcompat.app.AlertDialog;
import androidx.appcompat.app.AppCompatActivity;
import androidx.constraintlayout.widget.ConstraintLayout;
import androidx.core.app.ActivityCompat;
import androidx.core.content.ContextCompat;

import com.rstarx.hexrays.R;
import com.rstarx.hexrays.launcher.config.Config;
import com.rstarx.hexrays.launcher.util.*;

import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.opengles.GL10;

public class SplashActivity extends AppCompatActivity {
    private static final String TAG = "SplashActivity";
    private static final int ALL_FILES_REQUEST_CODE = 2222;
    private static final int BASIC_PERMISSION_CODE = 1111;

    // สิทธิ์ไมโครโฟน (พื้นที่จัดเก็บจะใช้ MANAGE_EXTERNAL_STORAGE แยกต่างหากสำหรับ Android 11+)
    private final String[] basicPermissions = {
            "android.permission.RECORD_AUDIO"
    };

    public int mGpuType = 2;
    public Messenger mMessenger;
    public Messenger mService;
    private boolean mIsBind = false;
    private TextView statusText;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_splash);

        statusText = findViewById(R.id.br_ls_progress2);

        Config.currentContext = this;
        mMessenger = new Messenger(new IncomingHandler(Looper.getMainLooper()));

        // 2. ตรวจสอบเน็ตและ GPU
        if (Util.isNetworkConnected(this)) {
            detectGpuAndStart();
        } else {
            showSimpleDialog("ไม่มีอินเทอร์เน็ต", "กรุณาเชื่อมต่ออินเทอร์เน็ตเพื่อโหลดข้อมูล", true);
        }
    }

    private void detectGpuAndStart() {
        GLSurfaceView mGlSurfaceView = new GLSurfaceView(this);
        mGlSurfaceView.setRenderer(new GLSurfaceView.Renderer() {
            @Override
            public void onSurfaceCreated(GL10 gl, EGLConfig config) {
                String ext = gl.glGetString(GL10.GL_EXTENSIONS);
                if (ext.contains("PVRTC")) mGpuType = 3;
                else if (ext.contains("DXT") || ext.contains("S3TC")) mGpuType = 1;
                else mGpuType = 2;

                Log.d(TAG, "GPU Type: " + mGpuType);

                // เริ่มขั้นตอนการเช็คสิทธิ์บน UI Thread
                runOnUiThread(() -> startPermissionSequence());
            }
            @Override public void onSurfaceChanged(GL10 gl, int w, int h) {}
            @Override public void onDrawFrame(GL10 gl) {}
        });

        ConstraintLayout layout = findViewById(R.id.loading);
        if (layout != null) {
            layout.addView(mGlSurfaceView, 10, 10);
        } else {
            startPermissionSequence();
        }
    }

    private void startPermissionSequence() {
        if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.R) {
            // สำหรับ Samsung Android 11, 12, 13, 14
            if (Environment.isExternalStorageManager()) {
                checkBasicPermissions();
            } else {
                showAllFilesDialog();
            }
        } else {
            // สำหรับ Android รุ่นเก่า
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
            startUpdateService();
        }
    }

    @Override
    protected void onActivityResult(int requestCode, int resultCode, Intent data) {
        super.onActivityResult(requestCode, resultCode, data);
        if (requestCode == ALL_FILES_REQUEST_CODE) {
            // แก้ปัญหา Samsung อัปเดตสิทธิ์ช้า: หน่วงเวลา 1 วินาทีก่อนเช็คสถานะใหม่
            new Handler(Looper.getMainLooper()).postDelayed(() -> {
                if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.R) {
                    if (Environment.isExternalStorageManager()) {
                        Log.d(TAG, "สิทธิ์ไฟล์ผ่านแล้ว");
                        checkBasicPermissions();
                    } else {
                        Toast.makeText(this, "คุณยังไม่ได้อนุญาตสิทธิ์ไฟล์!", Toast.LENGTH_LONG).show();
                        showAllFilesDialog(); // ถามซ้ำจนกว่าจะยอมเปิด
                    }
                }
            }, 1000);
        }
    }

    @Override
    public void onRequestPermissionsResult(int requestCode, @NonNull String[] permissions, @NonNull int[] grantResults) {
        super.onRequestPermissionsResult(requestCode, permissions, grantResults);
        if (requestCode == BASIC_PERMISSION_CODE) {
            // ไม่ว่าจะอนุญาตไมค์หรือไม่ ให้ไปหน้าโหลดต่อเพื่อให้เกมรันได้
            startUpdateService();
        }
    }

    private void startUpdateService() {
        try {
            Intent intent = new Intent(this, UpdateService.class);
            mIsBind = bindService(intent, mConnection, Context.BIND_AUTO_CREATE);
        } catch (Exception e) {
            Log.e(TAG, "Service Bind Failed: " + e.getMessage());
            startActivity(new Intent(this, MainActivity.class));
            finish();
        }
    }

    private final ServiceConnection mConnection = new ServiceConnection() {
        public void onServiceConnected(ComponentName name, IBinder iBinder) {
            mService = new Messenger(iBinder);
            checkUpdate();
        }
        public void onServiceDisconnected(ComponentName name) {
            mService = null;
            mIsBind = false;
        }
    };

    public void checkUpdate() {
        if (mService == null) return;
        Message msg = Message.obtain(null, 0);
        Bundle data = new Bundle();
        data.putInt("gputype", mGpuType);
        msg.setData(data);
        msg.replyTo = mMessenger;
        try { mService.send(msg); } catch (RemoteException e) { e.printStackTrace(); }
    }

    private void showSimpleDialog(String title, String msg, boolean exit) {
        new AlertDialog.Builder(this).setTitle(title).setMessage(msg).setCancelable(false)
                .setPositiveButton("ตกลง", (d, id) -> { if(exit) finish(); }).show();
    }

    public class IncomingHandler extends Handler {
        public IncomingHandler(Looper looper) { super(looper); }
        @Override
        public void handleMessage(@NonNull Message msg) {
            if (msg.what == 5) {
                String status = msg.getData().getString("status", "");
                if ("UpdateRequired".equals(status)) {
                    startActivity(new Intent(SplashActivity.this, UpdateActivity.class));
                } else {
                    startActivity(new Intent(SplashActivity.this, MainActivity.class));
                }
                finish();
            }
        }
    }

    @Override
    protected void onDestroy() {
        super.onDestroy();
        if(mIsBind) {
            unbindService(mConnection);
            mIsBind = false;
        }
    }
}
