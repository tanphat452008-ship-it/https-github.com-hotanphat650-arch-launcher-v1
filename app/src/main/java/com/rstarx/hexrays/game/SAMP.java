package com.rstarx.hexrays.game;

import android.annotation.SuppressLint;
import android.os.Bundle;
import android.util.Log;
import android.view.KeyEvent;
import android.widget.Toast;

import com.google.firebase.crashlytics.FirebaseCrashlytics;
import com.joom.paranoid.Obfuscate;
import com.rstarx.hexrays.game.ui.AttachEdit;
import com.rstarx.hexrays.game.ui.CustomKeyboard;
import com.rstarx.hexrays.game.ui.LoadingScreen;
import androidx.activity.OnBackPressedCallback;
import com.rstarx.hexrays.game.ui.dialog.DialogManager;
import com.rstarx.hexrays.launcher.util.SignatureChecker;
import java.io.UnsupportedEncodingException;
import java.nio.charset.StandardCharsets;



@Obfuscate
public class SAMP extends GTASA implements CustomKeyboard.InputListener, HeightProvider.HeightListener {
    private static final String TAG = "SAMP";
    private static SAMP instance;


    private CustomKeyboard mKeyboard;
    private DialogManager mDialog;
    private AttachEdit mAttachEdit;
    private LoadingScreen mLoadingScreen;

    public static SAMP getInstance() {
        return instance;
    }

    // --- ส่วนการจัดการ Loading Screen ---

    private void showLoadingScreen() {
        runOnUiThread(new Runnable() {
            @Override
            public void run() {
                if (mLoadingScreen != null) {
                    mLoadingScreen.show(); // แสดง Layout
                    mLoadingScreen.startLoadingProcess(); // เริ่มสแกนไฟล์ data/ และ texdb/
                }
            }
        });
    }

    private void hideLoadingScreen() {
        runOnUiThread(new Runnable() {
            @Override
            public void run() {
                if (mLoadingScreen != null) {
                    mLoadingScreen.hide();
                }
            }
        });
    }

    // --- เมธอดเสริมอื่นๆ ---

    private void hideTab() {}
    private void setTab(int id, String name, int score, int ping) {}
    private void clearTab() {}

    public void setPauseState(boolean pause) {
        runOnUiThread(new Runnable() {
            @Override
            public void run() {
                if (pause) {
                    mDialog.hideWithoutReset();
                    mAttachEdit.hideWithoutReset();
                } else {
                    if (mDialog.isShow) mDialog.showWithOldContent();
                    if (mAttachEdit.isShow) mAttachEdit.showWithoutReset();
                }
            }
        });
    }

    public void exitGame() {
        FirebaseCrashlytics.getInstance().setCrashlyticsCollectionEnabled(false);
        finishAndRemoveTask();
        System.exit(0);
    }

    public void showDialog(int dialogId, int dialogTypeId, byte[] bArr, byte[] bArr2, byte[] bArr3, byte[] bArr4) {
        final String caption = new String(bArr, StandardCharsets.UTF_8);
        final String content = new String(bArr2, StandardCharsets.UTF_8);
        final String leftBtnText = new String(bArr3, StandardCharsets.UTF_8);
        final String rightBtnText = new String(bArr4, StandardCharsets.UTF_8);
        runOnUiThread(() -> {
            this.mDialog.show(dialogId, dialogTypeId, caption, content, leftBtnText, rightBtnText);
        });
    }

    private native void onInputEnd(byte[] str);

    @Override
    public void OnInputEnd(String str) {
        byte[] toReturn = null;
        try {
            toReturn = str.getBytes("windows-874");
        } catch (UnsupportedEncodingException e) {
            Log.e(TAG, "Encoding error");
        }

        try {
            onInputEnd(toReturn);
        } catch (UnsatisfiedLinkError e5) {
            Log.e(TAG, e5.getMessage());
        }
    }

    private void showKeyboard() {
        runOnUiThread(() -> mKeyboard.ShowInputLayout());
    }

    private void hideKeyboard() {
        runOnUiThread(() -> mKeyboard.HideInputLayout());
    }

    private void showEditObject() {
        runOnUiThread(() -> mAttachEdit.show());
    }

    private void hideEditObject() {
        runOnUiThread(() -> mAttachEdit.hide());
    }

    // --- Lifecycle ---

    @Override
    public void onCreate(Bundle savedInstanceState) {
        Log.i(TAG, "**** onCreate");
        super.onCreate(savedInstanceState);

        // ตรวจสอบ Signature ของ Launcher
        if (!SignatureChecker.isSignatureValid(this, getPackageName())) {
            Toast.makeText(this, "Use original launcher! No remake", Toast.LENGTH_LONG).show();
            finish();
            return;
        }

        // เริ่มต้น Initialize UI Components
        mKeyboard = new CustomKeyboard(this);
        mDialog = new DialogManager(this);
        mAttachEdit = new AttachEdit(this);
        // mHudManager = new com.rstarx.hexrays.game.ui.HudManager(this);
        // 1. สร้างหน้าจอโหลด
        mLoadingScreen = new LoadingScreen(this);

        // 2. สั่งโชว์และเริ่มสแกนไฟล์ทันที
        showLoadingScreen();



        instance = this;

        try {
            initAssetManager(getAssets());
            // ตำแหน่ง path หลักสำหรับโหลดไฟล์เกม
            String storagePath = getExternalFilesDir(null).getAbsolutePath() + "/";
            initializeSAMP(storagePath);
        } catch (UnsatisfiedLinkError e5) {
            Log.e(TAG, "Native library error: " + e5.getMessage());
        }

        // จัดการปุ่ม Back
        getOnBackPressedDispatcher().addCallback(this, new OnBackPressedCallback(true) {
            @Override
            public void handleOnBackPressed() {
                setEnabled(false);
                onEventBackPressed();
                setEnabled(true);
            }
        });
    }

    private native void initializeSAMP(String storagePath);
    public native void initAssetManager(android.content.res.AssetManager assetManager);

    @Override
    public void onStart() {
        super.onStart();
        Log.i(TAG, "**** onStart");
    }

    @Override
    public void onResume() {
        super.onResume();
    }

    public native void onEventBackPressed();

    @SuppressLint("GestureBackNavigation")
    @Override
    public boolean onKeyDown(int keyCode, KeyEvent event) {
        if (keyCode == KeyEvent.KEYCODE_BACK) {
            onEventBackPressed();
        }
        return super.onKeyDown(keyCode, event);
    }

    @Override
    public void onPause() {
        super.onPause();
    }

    @Override
    public void onStop() {
        super.onStop();
    }

    @Override
    public void onDestroy() {
        super.onDestroy();
    }

    @Override
    public void onHeightChanged(int orientation, int height) {
        // mKeyboard.onHeightChanged(height);
    }
}