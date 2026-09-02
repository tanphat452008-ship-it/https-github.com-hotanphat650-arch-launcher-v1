package com.rstarx.hexrays.launcher;

import android.content.Context;
import android.content.Intent;
import android.net.Uri;
import android.os.Bundle;
import android.util.Log;
import android.view.View;
import android.view.inputmethod.InputMethodManager;
import android.widget.Button;
import android.widget.EditText;
import android.widget.ImageView;
import android.widget.Toast;

import androidx.appcompat.app.AppCompatActivity;

import com.bumptech.glide.Glide;
import com.rstarx.hexrays.R;
import com.rstarx.hexrays.game.SAMP;
import com.rstarx.hexrays.launcher.util.ConfigValidator;
import com.rstarx.hexrays.launcher.util.SAMPServerInfo;

import org.ini4j.Wini;
import org.json.JSONObject;

import java.io.File;
import java.io.IOException;
import java.util.ArrayList;

import okhttp3.Call;
import okhttp3.Callback;
import okhttp3.OkHttpClient;
import okhttp3.Request;
import okhttp3.Response;

public class MainActivity extends AppCompatActivity {

    private EditText mNickName;
    private Button btnConnect, btnLogout;
    private Wini mWini;
    private File iniFile;

    // URL API (สำหรับดึงรูปภาพและลิงก์เท่านั้น)
    private final String API_URL = "http://103.216.158.30/test/api.json";

    // --- ส่วนแก้ Error ห้ามลบ ---
    private ArrayList<SAMPServerInfo> mEmptyList = new ArrayList<>();
    public ArrayList<SAMPServerInfo> getServerList() { return mEmptyList; }
    public ArrayList<SAMPServerInfo> getFavoriteServerList() { return mEmptyList; }
    public void refreshFavoriteServers() { }

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        // Đã xóa phần SignatureChecker gây đóng app lập tức

        clearModelCache();

        mNickName = findViewById(R.id.settings_nickname);
        btnConnect = findViewById(R.id.server_connect);
        btnLogout = findViewById(R.id.logout_button);

        setupSettingsFile();

        // เริ่มโหลดรูปภาพและลิ้งก์จาก JSON
        fetchApiImagesAndLinks();

        btnConnect.setOnClickListener(v -> startGame());
        if (btnLogout != null) {
            btnLogout.setOnClickListener(v -> {
                finishAffinity();
                System.exit(0);
            });
        }
    }

    private void fetchApiImagesAndLinks() {
        OkHttpClient client = new OkHttpClient();
        Request request = new Request.Builder().url(API_URL).build();

        client.newCall(request).enqueue(new Callback() {
            @Override
            public void onFailure(Call call, IOException e) {
                Log.e("SAMP_API", "Load Failed: " + e.getMessage());
            }

            @Override
            public void onResponse(Call call, Response response) throws IOException {
                if (response.isSuccessful() && response.body() != null) {
                    try {
                        String jsonData = response.body().string();
                        JSONObject json = new JSONObject(jsonData);

                        // ดึงเฉพาะ Image URL และ Link URL (ไม่ดึง Title ภาษาไทย)
                        JSONObject fb = json.getJSONObject("facebook");
                        final String fbImg = fb.getString("image");
                        final String fbLink = fb.getString("url");

                        JSONObject dc = json.getJSONObject("discord");
                        final String dcImg = dc.getString("image");
                        final String dcLink = dc.getString("url");

                        runOnUiThread(() -> {
                            updateImagesAndLinks(fbImg, fbLink, dcImg, dcLink);
                        });

                    } catch (Exception e) {
                        e.printStackTrace();
                    }
                }
            }
        });
    }

    private void updateImagesAndLinks(String fbImg, String fbLink, String dcImg, String dcLink) {
        // โหลดรูป Facebook และตั้งค่าคลิก (ข้อความยังคงเดิมตาม XML)
        ImageView fbIv = findViewById(R.id.img_fb);
        if (fbIv != null) {
            Glide.with(this).load(fbImg).centerCrop().into(fbIv);
        }
        View fbCard = findViewById(R.id.btt_fb);
        if (fbCard != null) {
            fbCard.setOnClickListener(v -> openBrowser(fbLink));
        }

        // โหลดรูป Discord และตั้งค่าคลิก (ข้อความยังคงเดิมตาม XML)
        ImageView dcIv = findViewById(R.id.img_discord);
        if (dcIv != null) {
            Glide.with(this).load(dcImg).centerCrop().into(dcIv);
        }
        View dcCard = findViewById(R.id.btt_discord);
        if (dcCard != null) {
            dcCard.setOnClickListener(v -> openBrowser(dcLink));
        }
    }

    private void openBrowser(String url) {
        try {
            startActivity(new Intent(Intent.ACTION_VIEW, Uri.parse(url)));
        } catch (Exception e) {
            Toast.makeText(this, "ไม่สามารถเปิดลิงก์ได้", Toast.LENGTH_SHORT).show();
        }
    }

    private void setupSettingsFile() {
        ConfigValidator.validateConfigFiles(this);
        iniFile = new File(getExternalFilesDir(null) + "/SAMP/settings.ini");
        try {
            if (!iniFile.exists()) {
                iniFile.getParentFile().mkdirs();
                iniFile.createNewFile();
            }
            mWini = new Wini(iniFile);
            String nn = mWini.get("client", "name");
            if (nn != null) {
                mNickName.setText(nn);
                mNickName.setSelection(mNickName.getText().length());
            }
        } catch (Exception e) {
            Log.e("SAMP", "Init Error: " + e.getMessage());
        }
    }

    private void startGame() {
        String finalName = mNickName.getText().toString().trim();
        if (finalName.length() < 3) return;
        try {
            mWini.put("client", "name", finalName);
            mWini.store();
            hideKeyboard(this);
            startActivity(new Intent(MainActivity.this, SAMP.class));
            finish();
        } catch (IOException e) { }
    }

    public void hideKeyboard(android.app.Activity activity) {
        View view = (activity != null) ? activity.getCurrentFocus() : this.getCurrentFocus();
        if (view != null) {
            InputMethodManager imm = (InputMethodManager) getSystemService(Context.INPUT_METHOD_SERVICE);
            imm.hideSoftInputFromWindow(view.getWindowToken(), 0);
        }
    }

    private void clearModelCache() {
        try {
            String path = getExternalFilesDir(null).toString();
            new File(path + "/CINFO.BIN").delete();
            new File(path + "/models/MINFO.BIN").delete();
        } catch (Exception ignored) { }
    }
}
