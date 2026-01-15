package com.byreytiz.launcher.activity;

import android.Manifest;
import android.os.Bundle;
import android.os.Environment;
import android.content.pm.PackageManager;
import android.text.Editable;
import android.text.TextWatcher;
import android.view.KeyEvent;
import android.view.inputmethod.EditorInfo;
import android.widget.*;
import androidx.appcompat.widget.AppCompatButton;
import androidx.appcompat.app.AppCompatActivity;
import android.content.Intent;
import android.net.Uri;
import android.view.View;
import android.view.animation.AnimationUtils;
import android.view.animation.Animation;

import com.byreytiz.game.R;
import com.byreytiz.weikton.reg.Preferences;
import com.byreytiz.launcher.other.FileChecker;

import org.ini4j.Wini;

import java.io.File;
import java.io.IOException;
import java.util.*;
import java.util.regex.Pattern;

public class MainActivity extends AppCompatActivity implements FileChecker.CheckListener {

    EditText nickname;
    ImageButton ib_info;
    AppCompatButton buttonPlay;
    AppCompatButton buttonUpdate;

    private FileChecker fileChecker;
    private boolean needsUpdate = false;

    // Pattern để kiểm tra nickname (chỉ chữ cái Latin, không số và ký hiệu)
    private static final Pattern NICKNAME_PATTERN = Pattern.compile("^[a-zA-Z]+$");

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.fragment_main);

        // Check permissions
        if (checkSelfPermission(Manifest.permission.READ_EXTERNAL_STORAGE) == PackageManager.PERMISSION_DENIED ||
                checkSelfPermission(Manifest.permission.WRITE_EXTERNAL_STORAGE) == PackageManager.PERMISSION_DENIED ||
                checkSelfPermission(Manifest.permission.RECORD_AUDIO) == PackageManager.PERMISSION_DENIED) {
            requestPermissions(new String[]{Manifest.permission.READ_EXTERNAL_STORAGE,
                    Manifest.permission.WRITE_EXTERNAL_STORAGE, Manifest.permission.RECORD_AUDIO}, 1000);
        }

        initViews();
        setupClickListeners();
        loadNickname();

        // Initialize file checker
        fileChecker = new FileChecker(this);

        // Check for updates
        checkForUpdates();
    }

    private void initViews() {
        nickname = findViewById(R.id.edit_text_name);
        ib_info = findViewById(R.id.ib_info);
        buttonPlay = findViewById(R.id.button_play);
        buttonUpdate = findViewById(R.id.button_update_game);

        // Initially disable play button
        buttonPlay.setEnabled(false);
        buttonPlay.setText("Đang kiểm tra...");
    }

    private void setupClickListeners() {
        Animation animation = AnimationUtils.loadAnimation(this, R.anim.button_click);

        buttonPlay.setOnClickListener(v -> {
            v.startAnimation(animation);
            Timer t = new Timer();
            t.schedule(new TimerTask(){
                @Override
                public void run() {
                    onClickPlay();
                }
            }, 200L);
        });

        buttonUpdate.setOnClickListener(v -> {
            v.startAnimation(animation);
            Timer t = new Timer();
            t.schedule(new TimerTask(){
                @Override
                public void run() {
                    startActivity(new Intent(MainActivity.this, LoaderActivity.class));
                }
            }, 200L);
        });

        ib_info.setOnClickListener(v -> {
            v.startAnimation(animation);
            TextView info_nick = findViewById(R.id.text_view_info_about_nickname);
            if(info_nick.getVisibility() == View.INVISIBLE)
                info_nick.setVisibility(View.VISIBLE);
            else
                info_nick.setVisibility(View.INVISIBLE);
        });

        findViewById(R.id.button_discord).setOnClickListener(v -> {
            v.startAnimation(animation);
            Timer t = new Timer();
            t.schedule(new TimerTask(){
                @Override
                public void run() {
                    startActivity(new Intent("android.intent.action.VIEW",
                            Uri.parse("https://discord.gg/sarp2025")));
                }
            }, 200L);
        });

        setupNicknameEditor();
    }

    private void setupNicknameEditor() {
        nickname.setOnEditorActionListener((v, actionId, event) -> {
            if (actionId == EditorInfo.IME_ACTION_DONE ||
                    (event != null && event.getKeyCode() == KeyEvent.KEYCODE_ENTER)) {
                saveNickname();
            }
            return false;
        });

        nickname.addTextChangedListener(new TextWatcher() {
            @Override
            public void beforeTextChanged(CharSequence s, int start, int count, int after) {}

            @Override
            public void onTextChanged(CharSequence s, int start, int before, int count) {
                saveNickname();
            }

            @Override
            public void afterTextChanged(Editable s) {}
        });
    }

    private void checkForUpdates() {
        fileChecker.checkFiles(this);
    }

    // FileChecker.CheckListener implementation
    @Override
    public void onNeedUpdate(List<FileChecker.GameFile> filesToUpdate) {
        runOnUiThread(() -> {
            needsUpdate = true;
            buttonPlay.setEnabled(false);
            buttonPlay.setText("Cần cập nhật");
            buttonUpdate.setVisibility(View.VISIBLE);

            showToast("Tìm thấy " + filesToUpdate.size() + " file(s) cần cập nhật!");
        });
    }

    @Override
    public void onUpToDate() {
        runOnUiThread(() -> {
            needsUpdate = false;

            if (FileChecker.isGameInstalled()) {
                buttonPlay.setEnabled(true);
                buttonPlay.setText("Tham gia");
                buttonUpdate.setVisibility(View.GONE);
            } else {
                buttonPlay.setEnabled(false);
                buttonPlay.setText("Chưa cài đặt");
                buttonUpdate.setVisibility(View.VISIBLE);
            }
        });
    }

    @Override
    public void onError(String error) {
        runOnUiThread(() -> {
            // Allow play if game is installed even if check failed
            if (FileChecker.isGameInstalled()) {
                buttonPlay.setEnabled(true);
                buttonPlay.setText("Tham gia");
                needsUpdate = false;
            } else {
                buttonPlay.setEnabled(false);
                buttonUpdate.setVisibility(View.VISIBLE);
            }

            showToast("Lỗi kiểm tra: " + error);
        });
    }

    @Override
    public void onRequestPermissionsResult(int requestCode, String[] permissions, int[] grantResults) {
        super.onRequestPermissionsResult(requestCode, permissions, grantResults);
        if (requestCode == 1000) {
            checkForUpdates();
        } else {
            requestPermissions(new String[]{Manifest.permission.READ_EXTERNAL_STORAGE,
                    Manifest.permission.WRITE_EXTERNAL_STORAGE, Manifest.permission.RECORD_AUDIO}, 1000);
        }
    }

    public void onClickPlay() {
        runOnUiThread(() -> {
            // Kiểm tra nickname trước khi vào game
            String nickText = nickname.getText().toString().trim();

            if (!validateNickname(nickText)) {
                return; // Không cho vào game nếu nickname không hợp lệ
            }

            if (needsUpdate) {
                showToast("Vui lòng cập nhật game trước!");
                return;
            }

            if (FileChecker.isGameInstalled()) {
                startActivity(new Intent(this, com.byreytiz.game.core.GTASA.class));
            } else {
                showToast("Game chưa cài đặt. Nhấn 'Cập nhật'");
                startActivity(new Intent(this, LoaderActivity.class));
            }
        });
    }

    private boolean validateNickname(String nickname) {
        if (nickname.isEmpty()) {
            showToast("Vui lòng nhập tên nhân vật!");
            this.nickname.requestFocus();
            return false;
        }

        if (nickname.length() < 3 || nickname.length() > 23) {
            showToast("Tên nhân vật phải có độ dài từ 3-23 ký tự!");
            this.nickname.requestFocus();
            return false;
        }

        if (!NICKNAME_PATTERN.matcher(nickname).matches()) {
            showToast("Tên nhân vật chỉ được chứa các ký tự Latin (a-z, A-Z), không có số và ký hiệu!");
            this.nickname.requestFocus();
            return false;
        }

        // Kiểm tra từ ngữ xúc phạm (có thể thêm danh sách từ cấm)
        String lowerNick = nickname.toLowerCase();
        String[] bannedWords = {"admin", "fuck", "shit", "damn", "hell", "bitch", "ass", "dick", "pussy", "gay", "lesbian"};
        for (String word : bannedWords) {
            if (lowerNick.contains(word)) {
                showToast("Tên nhân vật không được chứa từ ngữ xúc phạm!");
                this.nickname.requestFocus();
                return false;
            }
        }

        return true;
    }

    private void saveNickname() {
        String nickText = nickname.getText().toString().trim();
        if (nickText.isEmpty()) {
            return; // Không hiển thị lỗi khi đang gõ
        }

        try {
            File settingsFile = new File(Environment.getExternalStorageDirectory() + "/bytaylyx/SAMP/settings.ini");
            if (!settingsFile.exists()) {
                settingsFile.getParentFile().mkdirs();
                settingsFile.createNewFile();
            }

            Wini w = new Wini(settingsFile);
            w.put("client", "name", nickText);
            w.store();

            Preferences.setNick(nickText);

            // Chỉ hiển thị thông báo lưu thành công nếu nickname hợp lệ
            if (validateNicknameQuiet(nickText)) {
                showToast("Thiết lập tên nhân vật thành công!");
            }

        } catch (IOException e) {
            e.printStackTrace();
            showToast("Lỗi thiết lập tên nhân vật!");
        }
    }

    // Phiên bản im lặng của validate (không hiển thị toast)
    private boolean validateNicknameQuiet(String nickname) {
        if (nickname.isEmpty()) return false;
        if (nickname.length() < 3 || nickname.length() > 23) return false;
        if (!NICKNAME_PATTERN.matcher(nickname).matches()) return false;

        String lowerNick = nickname.toLowerCase();
        String[] bannedWords = {"admin", "fuck", "shit", "damn", "hell", "bitch", "ass", "dick", "pussy", "gay", "lesbian"};
        for (String word : bannedWords) {
            if (lowerNick.contains(word)) return false;
        }

        return true;
    }

    private void loadNickname() {
        try {
            File settingsFile = new File(Environment.getExternalStorageDirectory() + "/bytaylyx/SAMP/settings.ini");
            if (settingsFile.exists()) {
                Wini w = new Wini(settingsFile);
                String savedNick = w.get("client", "name");
                if (savedNick != null && !savedNick.isEmpty()) {
                    nickname.setText(savedNick);
                    Preferences.setNick(savedNick);
                }
            }
        } catch (IOException e) {
            e.printStackTrace();
        }
    }

    private void showToast(String message) {
        Toast.makeText(this, message, Toast.LENGTH_SHORT).show();
    }

    @Override
    protected void onResume() {
        super.onResume();
        // Re-check when returning to activity
        checkForUpdates();
    }
}