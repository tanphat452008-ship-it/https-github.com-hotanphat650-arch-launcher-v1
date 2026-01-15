package com.byreytiz.game.gui;

import android.app.Activity;
import android.os.Environment;
import android.os.Handler;
import android.view.View;
import android.widget.Button;
import android.widget.TextView;
import android.widget.Toast;

import androidx.constraintlayout.widget.ConstraintLayout;
import com.byreytiz.game.R;
import com.nvidia.devtech.NvEventQueueActivity;

import org.ini4j.Wini;

import java.io.File;
import java.io.IOException;
import java.util.Timer;

public class PausMenu {
    ConstraintLayout pon;
    TextView avtor, textPausa, textNick;
    Button play, settings, button3;
    Timer timer;

    public CountUpTimer countUpTimer;


    public PausMenu(Activity activity) {
        pon = activity.findViewById(R.id.PausMenu);
        avtor = activity.findViewById(R.id.Avtor);
        pon.setVisibility(View.GONE);
        play = activity.findViewById(R.id.button_playgo2);
        settings = activity.findViewById(R.id.button_clean_game2);
        button3 = activity.findViewById(R.id.button3);
        textPausa = activity.findViewById(R.id.textPausa);
        textNick = activity.findViewById(R.id.textNick);
        textPausa.setText(String.valueOf(textPausa));

        timer = new Timer();

        avtor.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                Toast.makeText(activity, "afk mode:on", Toast.LENGTH_SHORT).show();
            }
        });
        play.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                Hide();
            }
        });
        settings.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                NvEventQueueActivity.getInstance().showSettingsMenu();
                Toast.makeText(activity, "Подождите", Toast.LENGTH_SHORT).show();
                Hide();
            }
        });


        button3.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                Toast.makeText(activity, "В разработке", Toast.LENGTH_SHORT).show();
            }
        });

    }
    private void InitLogic() {
        try {
            Wini w = new Wini(new File(Environment.getExternalStorageDirectory() + "/bytaylyx/SAMP/settings.ini"));
            textNick.setText(w.get("client", "name"));
            w.store();
        } catch (IOException e) {
            e.printStackTrace();
        }
    }

    public void Show() {
        pon.setVisibility(View.VISIBLE);
        startTimer();
        InitLogic();
    }
    private void startTimer() {
        countUpTimer = new CountUpTimer(textPausa);
        countUpTimer.start();
    }
    private void stopTimer() {
        if (countUpTimer != null) {
            countUpTimer.stop();
            textPausa.setText("00:00");
            textPausa.setGravity(View.TEXT_ALIGNMENT_CENTER);
        }
    }
    public void Hide() {
        pon.setVisibility(View.GONE);
        stopTimer();
    }
    private class CountUpTimer {
        private TextView textView;
        private Handler handler;
        private int seconds;

        public CountUpTimer(TextView textView) {
            this.textView = textView;
            this.seconds = 0;
            this.handler = new Handler();
        }
        public void start() {
            handler.postDelayed(timerRunnable, 1000);
        }
        private Runnable timerRunnable = new Runnable() {
            @Override
            public void run() {
                seconds++;
                updateTimer();
                handler.postDelayed(this, 1000);
            }
        };
        public void stop() {
            handler.removeCallbacks(timerRunnable);
        }
        private void updateTimer() {
            int minutes = seconds / 60;
            int remainingSeconds = seconds % 60;
            String time = String.format("%02d:%02d", minutes, remainingSeconds);
            textView.setText(time);
        }
    }
}