package com.byreytiz.game.gui;

import android.app.Activity;
import android.view.View;
import android.view.animation.AnimationUtils;
import android.widget.ImageView;
import android.widget.ProgressBar;
import android.widget.TextView;

import androidx.constraintlayout.widget.ConstraintLayout;

import com.nvidia.devtech.NvEventQueueActivity;
import com.byreytiz.game.R;
import com.byreytiz.game.gui.util.Utils;

import java.text.DecimalFormat;
import java.text.DecimalFormatSymbols;
import java.util.ArrayList;
import java.util.Formatter;

public class HudManager {
    public Activity activity;
    public ConstraintLayout hud_layout;

    private final NvEventQueueActivity mContext = null;

    public TextView hud_money2;
    public TextView hud_bank;

    public TextView progressFood;
    public TextView progressWater;
    public TextView progressStress;
    public TextView textPoliceOnline;
    public TextView textPoliceMedic;

    public TextView hud_username;

    public ImageView hud_weapon;

    public ImageView hud_radar;

    public ImageView hud_x2;

    public ImageView hud_menu;
    public ImageView hud_micro;

    public ImageView hud_gps;

    public ImageView hud_zona;

    public ImageView hud_donate;

    public ArrayList<ImageView> hud_wanted;

    public TextView hud_health;
    public TextView hud_armor;

    public ImageView gbutton;


    public HudManager(Activity aactivity) {
        activity = aactivity;
        hud_layout = aactivity.findViewById(R.id.hud_main);
        hud_layout.setVisibility(View.GONE);

        hud_username = aactivity.findViewById(R.id.hud_username);

        hud_health = aactivity.findViewById(R.id.hud_health);
        hud_armor = aactivity.findViewById(R.id.hud_armor);

        hud_radar = aactivity.findViewById(R.id.radar_hude);

        hud_micro = aactivity.findViewById(R.id.imageView14);

        hud_gps = aactivity.findViewById(R.id.imageView16);
        hud_zona = aactivity.findViewById(R.id.grzona);
        hud_x2 = aactivity.findViewById(R.id.imageView17);

        progressFood = aactivity.findViewById(R.id.progressFood);
        progressWater = aactivity.findViewById(R.id.progressWater);
        progressStress = aactivity.findViewById(R.id.progressStress);
        textPoliceOnline = aactivity.findViewById(R.id.textPoliceOnline);
        textPoliceMedic = aactivity.findViewById(R.id.textPoliceMedic);

        hud_money2 = aactivity.findViewById(R.id.hud_money2);
        hud_bank = aactivity.findViewById(R.id.hud_bank);
        hud_weapon = aactivity.findViewById(R.id.hud_weapon);
        hud_menu = aactivity.findViewById(R.id.hud_menu);

        hud_wanted = new ArrayList<>();
        hud_wanted.add(activity.findViewById(R.id.hud_star_1));
        hud_wanted.add(activity.findViewById(R.id.hud_star_2));
        hud_wanted.add(activity.findViewById(R.id.hud_star_3));
        hud_wanted.add(activity.findViewById(R.id.hud_star_4));
        hud_wanted.add(activity.findViewById(R.id.hud_star_5));
        hud_wanted.add(activity.findViewById(R.id.hud_star_6));
        hud_micro.setOnClickListener( view -> {
            view.startAnimation(AnimationUtils.loadAnimation(aactivity, R.anim.button_click));
        });
        /*hud_radar.setOnClickListener(view -> {
            NvEventQueueActivity.getInstance().showPausMenu();

        });*/

        hud_menu.setOnClickListener( view -> {
            view.startAnimation(AnimationUtils.loadAnimation(aactivity, R.anim.button_click));
            NvEventQueueActivity.getInstance().showMenuu();
            NvEventQueueActivity.getInstance().togglePlayer(1);
        });
    }

    public void UpdateHudInfo(int health, int armour, int food, int water, int weaponid, int ammo, int playerid, int money, int wanted, int stress, int policeOnline, int policeMedic, int bankAccount, String username)
    {
        // Xử lý tất cả parameters
        if(hud_health != null) {
            hud_health.setText(String.valueOf(health));
        }

        if(hud_armor != null) {
            hud_armor.setText(String.valueOf(health));
        }

        // Update food
        if(progressFood != null) {
            progressFood.setText(String.valueOf(food));
        }

        // Update water
        if(progressWater != null) {
            progressWater.setText(String.valueOf(water));
        }

        // Update stress
        if(progressStress != null) {
            progressStress.setText(String.valueOf(stress));
        }

        // Update police info
        if(textPoliceOnline != null) {
            textPoliceOnline.setText(String.valueOf(policeOnline));
        }

        if(textPoliceMedic != null) {
            textPoliceMedic.setText(String.valueOf(policeMedic));
        }

        if(hud_username != null) {
            hud_username.setText(username);
        }

        // Money format
        DecimalFormat formatter = new DecimalFormat();
        DecimalFormatSymbols symbols = DecimalFormatSymbols.getInstance();
        symbols.setGroupingSeparator('.');
        formatter.setDecimalFormatSymbols(symbols);
        String s = formatter.format(money);
        hud_money2.setText(s);

        String sss = formatter.format(bankAccount);
        hud_bank.setText(sss);

        // Weapon
        int id = activity.getResources().getIdentifier(new Formatter().format("weapon_%d", Integer.valueOf(weaponid)).toString(), "drawable", activity.getPackageName());
        hud_weapon.setImageResource(id);

        // Wanted level
        if(wanted > 6) wanted = 6;
        for (int i2 = 0; i2 < wanted; i2++) {
            hud_wanted.get(i2).setBackgroundResource(R.drawable.ic_y_star);
        }
    }

    public void ShowGps()
    {
        Utils.ShowLayout(hud_gps, false);
    }

    public void HideGps()
    {
        Utils.HideLayout(hud_gps, false);
    }

    public void ShowX2()
    {
        Utils.ShowLayout(hud_x2, false);
    }

    public void HideX2()
    {
        Utils.HideLayout(hud_x2, false);
    }

    public void ShowZona()
    {
        Utils.ShowLayout(hud_zona, false);
    }

    public void HideZona()
    {
        Utils.HideLayout(hud_zona, false);
    }

    public void ShowRadar()
    {
        Utils.ShowLayout(hud_radar, true);
    }

    public void HideRadar()
    {
        Utils.HideLayout(hud_radar, false);
    }

    public void ShowHud()
    {
        Utils.ShowLayout(hud_layout, true);
        //Utils.ShowLayout(hud_micro, false);
    }

    public void HideHud()
    {
        Utils.HideLayout(hud_layout, false);
        //Utils.HideLayout(hud_micro, false);
    }

}
