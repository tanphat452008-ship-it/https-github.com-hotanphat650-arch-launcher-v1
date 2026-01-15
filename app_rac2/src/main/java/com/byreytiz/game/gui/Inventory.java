package com.byreytiz.game.gui.inventory;

import android.app.Activity;
import android.view.View;
import android.view.animation.AnimationUtils;
import android.widget.FrameLayout;
import android.widget.ImageView;
import android.widget.LinearLayout;
import android.widget.RelativeLayout;
import android.widget.TextView;

import androidx.constraintlayout.widget.ConstraintLayout;

import com.byreytiz.game.R;
import com.byreytiz.game.gui.util.Utils;
import com.byreytiz.weikton.reg.Preferences;
import com.nvidia.devtech.NvEventQueueActivity;

public class Inventory {
    public int activeSlot;
    public Activity activity;
    public ImageView button_close;
    public RelativeLayout call_donate;
    public RelativeLayout call_menu;
    public RelativeLayout call_menuactions;
    public RelativeLayout call_pass;
    public RelativeLayout call_stats;
    public LinearLayout change_button;
    public LinearLayout delete_button;
    public ImageView img_inv_item;
    public ImageView img_slot1;
    public ImageView img_slot2;
    public ImageView img_slot3;
    public ImageView img_slot4;
    public ImageView img_slot5;
    public ImageView img_slot6;
    public ImageView img_slot7;
    public ImageView img_slot8;
    public FrameLayout inv_bg_text_item;
    public ConstraintLayout inventory_reytiz;
    public TextView nick_id;
    public FrameLayout sim_butt;
    public FrameLayout slot1;
    public int slot1ID;
    public int slot1arg;
    public FrameLayout slot2;
    public int slot2ID;
    public int slot2arg;
    public FrameLayout slot3;
    public int slot3ID;
    public int slot3arg;
    public FrameLayout slot4;
    public int slot4ID;
    public int slot4arg;
    public FrameLayout slot5;
    public int slot5ID;
    public int slot5arg;
    public FrameLayout slot6;
    public int slot6ID;
    public int slot6arg;
    public FrameLayout slot7;
    public int slot7ID;
    public int slot7arg;
    public FrameLayout slot8;
    public int slot8ID;
    public int slot8arg;
    public TextView text_arg1;
    public TextView text_arg2;
    public TextView text_arg3;
    public TextView text_arg4;
    public TextView text_arg5;
    public TextView text_arg6;
    public TextView text_arg7;
    public TextView text_arg8;
    public TextView text_item_inv;
    public TextView text_item_inv_title;
    public LinearLayout use_button;
    public TextView weight_item_inv;

    public Inventory(Activity aactivity) {
        this.activity = aactivity;
        this.button_close = activity.findViewById(R.id.exit_button);
        this.call_donate = activity.findViewById(R.id.inv_line_donate);
        this.call_menu = activity.findViewById(R.id.inv_line_menu);
        this.call_menuactions = activity.findViewById(R.id.inv_line_menuactions);
        this.call_pass = activity.findViewById(R.id.inv_line_pass);
        this.call_stats = activity.findViewById(R.id.inv_line_stats);
        this.change_button = activity.findViewById(R.id.obmen_inv_but);
        this.delete_button = activity.findViewById(R.id.delete_inv_button);
        this.img_inv_item = activity.findViewById(R.id.img_inv_item);
        this.img_slot1 = activity.findViewById(R.id.img_slot1_inv);
        this.img_slot2 = activity.findViewById(R.id.img_slot2_inv);
        this.img_slot3 = activity.findViewById(R.id.img_slot3_inv);
        this.img_slot4 = activity.findViewById(R.id.img_slot4_inv);
        this.img_slot5 = activity.findViewById(R.id.img_slot5_inv);
        this.img_slot6 = activity.findViewById(R.id.img_slot6_inv);
        this.img_slot7 = activity.findViewById(R.id.img_slot7_inv);
        this.img_slot8 = activity.findViewById(R.id.img_slot8_inv);
        this.inv_bg_text_item = activity.findViewById(R.id.inv_bg_text_item);
        this.nick_id = activity.findViewById(R.id.nick_text);
        this.sim_butt = activity.findViewById(R.id.element_row_inv_1);
        this.slot1 = activity.findViewById(R.id.inv_slot1);
        this.slot2 = activity.findViewById(R.id.inv_slot2);
        this.slot3 = activity.findViewById(R.id.inv_slot3);
        this.slot4 = activity.findViewById(R.id.inv_slot4);
        this.slot5 = activity.findViewById(R.id.inv_slot5);
        this.slot6 = activity.findViewById(R.id.inv_slot6);
        this.slot7 = activity.findViewById(R.id.inv_slot7);
        this.slot8 = activity.findViewById(R.id.inv_slot8);
        this.text_arg1 = activity.findViewById(R.id.text_arg1_inv);
        this.text_arg2 = activity.findViewById(R.id.text_arg2_inv);
        this.text_arg3 = activity.findViewById(R.id.text_arg3_inv);
        this.text_arg4 = activity.findViewById(R.id.text_arg4_inv);
        this.text_arg5 = activity.findViewById(R.id.text_arg5_inv);
        this.text_arg6 = activity.findViewById(R.id.text_arg6_inv);
        this.text_arg7 = activity.findViewById(R.id.text_arg7_inv);
        this.text_arg8 = activity.findViewById(R.id.text_arg8_inv);
        this.text_item_inv = activity.findViewById(R.id.text_item_inv);
        this.text_item_inv_title = activity.findViewById(R.id.text_item_inv_title);
        this.use_button = activity.findViewById(R.id.inv_use_button);
        this.weight_item_inv = activity.findViewById(R.id.weight_item_inv);
        this.inventory_reytiz = (ConstraintLayout) activity.findViewById(R.id.inventory_reytiz);
        Utils.HideLayout(this.inventory_reytiz, false);
        setListeners(activity);
    }

    public void setListeners(final Activity aactivity) {
        this.button_close.setOnClickListener(new View.OnClickListener() {
            public void onClick(View v) {
                Inventory.this.close(true);
            }
        });
        this.call_menu.setOnClickListener(new View.OnClickListener() {
            public void onClick(View v) {
                Inventory.this.close(false);
                NvEventQueueActivity.getInstance().sendInventoryClick(0);
            }
        });
        this.call_stats.setOnClickListener(new View.OnClickListener() {
            public void onClick(View v) {
                Inventory.this.close(false);
                NvEventQueueActivity.getInstance().sendInventoryClick(1);
            }
        });
        this.call_donate.setOnClickListener(new View.OnClickListener() {
            public void onClick(View v) {
                Inventory.this.close(false);
                NvEventQueueActivity.getInstance().sendInventoryClick(2);
            }
        });
        this.call_menuactions.setOnClickListener(new View.OnClickListener() {
            public void onClick(View v) {
                Inventory.this.close(false);
                NvEventQueueActivity.getInstance().showMenu();
            }
        });
        this.call_pass.setOnClickListener(new View.OnClickListener() {
            public void onClick(View v) {
                v.startAnimation(AnimationUtils.loadAnimation(aactivity, R.anim.button_click));
                NvEventQueueActivity.getInstance().showNotification(2, "BLACK PASS находится в разработке!", 5, "", "");
            }
        });
        this.sim_butt.setOnClickListener(new View.OnClickListener() {
            public void onClick(View v) {
                Inventory.this.clear();
                v.startAnimation(AnimationUtils.loadAnimation(aactivity, R.anim.button_click));
                Inventory.this.sim_butt.setBackgroundResource(R.drawable.inv_bg_active_item_in_list);
                Inventory.this.showItemInfo(0.5f, R.drawable.inv_sim, "SIM-Карта", "SIM-Карта нужна для звонков и сообщений по\nтелефону");
            }
        });
        this.slot1.setOnClickListener(new View.OnClickListener() {
            public void onClick(View v) {
                Inventory.this.clear();
                if (Inventory.this.slot1ID > 0) {
                    v.startAnimation(AnimationUtils.loadAnimation(aactivity, R.anim.button_click));
                    Inventory.this.slot1.setBackgroundResource(R.drawable.inv_bg_active_item_in_list);
                    Inventory inventory = Inventory.this;
                    inventory.showSlotInfo(inventory.slot1ID, Inventory.this.slot1arg);
                    Inventory.this.activeSlot = 1;
                }
            }
        });
        this.slot2.setOnClickListener(new View.OnClickListener() {
            public void onClick(View v) {
                Inventory.this.clear();
                if (Inventory.this.slot2ID > 0) {
                    v.startAnimation(AnimationUtils.loadAnimation(aactivity, R.anim.button_click));
                    Inventory.this.slot2.setBackgroundResource(R.drawable.inv_bg_active_item_in_list);
                    Inventory inventory = Inventory.this;
                    inventory.showSlotInfo(inventory.slot2ID, Inventory.this.slot2arg);
                    Inventory.this.activeSlot = 2;
                }
            }
        });
        this.slot3.setOnClickListener(new View.OnClickListener() {
            public void onClick(View v) {
                Inventory.this.clear();
                if (Inventory.this.slot3ID > 0) {
                    v.startAnimation(AnimationUtils.loadAnimation(aactivity, R.anim.button_click));
                    Inventory.this.slot3.setBackgroundResource(R.drawable.inv_bg_active_item_in_list);
                    Inventory inventory = Inventory.this;
                    inventory.showSlotInfo(inventory.slot3ID, Inventory.this.slot3arg);
                    Inventory.this.activeSlot = 3;
                }
            }
        });
        this.slot4.setOnClickListener(new View.OnClickListener() {
            public void onClick(View v) {
                Inventory.this.clear();
                if (Inventory.this.slot4ID > 0) {
                    v.startAnimation(AnimationUtils.loadAnimation(aactivity, R.anim.button_click));
                    Inventory.this.slot4.setBackgroundResource(R.drawable.inv_bg_active_item_in_list);
                    Inventory inventory = Inventory.this;
                    inventory.showSlotInfo(inventory.slot4ID, Inventory.this.slot4arg);
                    Inventory.this.activeSlot = 4;
                }
            }
        });
        this.slot5.setOnClickListener(new View.OnClickListener() {
            public void onClick(View v) {
                Inventory.this.clear();
                if (Inventory.this.slot5ID > 0) {
                    v.startAnimation(AnimationUtils.loadAnimation(aactivity, R.anim.button_click));
                    Inventory.this.slot5.setBackgroundResource(R.drawable.inv_bg_active_item_in_list);
                    Inventory inventory = Inventory.this;
                    inventory.showSlotInfo(inventory.slot5ID, Inventory.this.slot5arg);
                    Inventory.this.activeSlot = 5;
                }
            }
        });
        this.slot6.setOnClickListener(new View.OnClickListener() {
            public void onClick(View v) {
                Inventory.this.clear();
                if (Inventory.this.slot6ID > 0) {
                    v.startAnimation(AnimationUtils.loadAnimation(aactivity, R.anim.button_click));
                    Inventory.this.slot6.setBackgroundResource(R.drawable.inv_bg_active_item_in_list);
                    Inventory inventory = Inventory.this;
                    inventory.showSlotInfo(inventory.slot6ID, Inventory.this.slot6arg);
                    Inventory.this.activeSlot = 6;
                }
            }
        });
        this.slot7.setOnClickListener(new View.OnClickListener() {
            public void onClick(View v) {
                Inventory.this.clear();
                if (Inventory.this.slot7ID > 0) {
                    v.startAnimation(AnimationUtils.loadAnimation(aactivity, R.anim.button_click));
                    Inventory.this.slot7.setBackgroundResource(R.drawable.inv_bg_active_item_in_list);
                    Inventory inventory = Inventory.this;
                    inventory.showSlotInfo(inventory.slot7ID, Inventory.this.slot7arg);
                    Inventory.this.activeSlot = 7;
                }
            }
        });
        this.slot8.setOnClickListener(new View.OnClickListener() {
            public void onClick(View v) {
                Inventory.this.clear();
                if (Inventory.this.slot8ID > 0) {
                    v.startAnimation(AnimationUtils.loadAnimation(aactivity, R.anim.button_click));
                    Inventory.this.slot8.setBackgroundResource(R.drawable.inv_bg_active_item_in_list);
                    Inventory inventory = Inventory.this;
                    inventory.showSlotInfo(inventory.slot8ID, Inventory.this.slot8arg);
                    Inventory.this.activeSlot = 8;
                }
            }
        });
        this.use_button.setOnClickListener(new View.OnClickListener() {
            public void onClick(View v) {
                v.startAnimation(AnimationUtils.loadAnimation(aactivity, R.anim.button_click));
                switch (Inventory.this.activeSlot) {
                    case 1:
                        NvEventQueueActivity.getInstance().sendUseInventory(Inventory.this.slot1ID, 1);
                        return;
                    case 2:
                        NvEventQueueActivity.getInstance().sendUseInventory(Inventory.this.slot2ID, 2);
                        return;
                    case 3:
                        NvEventQueueActivity.getInstance().sendUseInventory(Inventory.this.slot3ID, 3);
                        return;
                    case 4:
                        NvEventQueueActivity.getInstance().sendUseInventory(Inventory.this.slot4ID, 4);
                        return;
                    case 5:
                        NvEventQueueActivity.getInstance().sendUseInventory(Inventory.this.slot5ID, 5);
                        return;
                    case 6:
                        NvEventQueueActivity.getInstance().sendUseInventory(Inventory.this.slot6ID, 6);
                        return;
                    case 7:
                        NvEventQueueActivity.getInstance().sendUseInventory(Inventory.this.slot7ID, 7);
                        return;
                    case 8:
                        NvEventQueueActivity.getInstance().sendUseInventory(Inventory.this.slot8ID, 8);
                        return;
                    default:
                        return;
                }
            }
        });
        this.change_button.setOnClickListener(new View.OnClickListener() {
            public void onClick(View v) {
                v.startAnimation(AnimationUtils.loadAnimation(aactivity, R.anim.button_click));
                NvEventQueueActivity.getInstance().showNotification(2, "Обмен находится в разработке!", 5, "", "");
            }
        });
        this.delete_button.setOnClickListener(new View.OnClickListener() {
            public void onClick(View v) {
                v.startAnimation(AnimationUtils.loadAnimation(aactivity, R.anim.button_click));
                switch (Inventory.this.activeSlot) {
                    case 1:
                        NvEventQueueActivity.getInstance().sendDelInventory(Inventory.this.slot1ID, 1);
                        return;
                    case 2:
                        NvEventQueueActivity.getInstance().sendDelInventory(Inventory.this.slot2ID, 2);
                        return;
                    case 3:
                        NvEventQueueActivity.getInstance().sendDelInventory(Inventory.this.slot3ID, 3);
                        return;
                    case 4:
                        NvEventQueueActivity.getInstance().sendDelInventory(Inventory.this.slot4ID, 4);
                        return;
                    case 5:
                        NvEventQueueActivity.getInstance().sendDelInventory(Inventory.this.slot5ID, 5);
                        return;
                    case 6:
                        NvEventQueueActivity.getInstance().sendDelInventory(Inventory.this.slot6ID, 6);
                        return;
                    case 7:
                        NvEventQueueActivity.getInstance().sendDelInventory(Inventory.this.slot7ID, 7);
                        return;
                    case 8:
                        NvEventQueueActivity.getInstance().sendDelInventory(Inventory.this.slot8ID, 8);
                        return;
                    default:
                        return;
                }
            }
        });
    }

    public void show() {
        Utils.ShowLayout(this.inventory_reytiz, true);
        NvEventQueueActivity.getInstance().hideHud();
        NvEventQueueActivity.getInstance().togglePlayer(1);
        TextView textView = this.nick_id;
        textView.setText(Preferences.getNick() + " [" + Preferences.GetID() + "]");
    }

    public void close(boolean isAnim) {
        Utils.HideLayout(this.inventory_reytiz, isAnim);
        NvEventQueueActivity.getInstance().showHud();
        NvEventQueueActivity.getInstance().togglePlayer(0);
    }

    public void clear() {
        this.sim_butt.setBackgroundResource(R.drawable.inv_bg_not_active_item_in_list);
        this.slot1.setBackgroundResource(R.drawable.inv_bg_not_active_item_in_list);
        this.slot2.setBackgroundResource(R.drawable.inv_bg_not_active_item_in_list);
        this.slot3.setBackgroundResource(R.drawable.inv_bg_not_active_item_in_list);
        this.slot4.setBackgroundResource(R.drawable.inv_bg_not_active_item_in_list);
        this.slot5.setBackgroundResource(R.drawable.inv_bg_not_active_item_in_list);
        this.slot6.setBackgroundResource(R.drawable.inv_bg_not_active_item_in_list);
        this.slot7.setBackgroundResource(R.drawable.inv_bg_not_active_item_in_list);
        this.slot8.setBackgroundResource(R.drawable.inv_bg_not_active_item_in_list);
        this.inv_bg_text_item.setVisibility(View.GONE);
        this.weight_item_inv.setVisibility(View.GONE);
        this.img_inv_item.setVisibility(View.GONE);
        this.text_item_inv_title.setVisibility(View.GONE);
        this.text_item_inv.setVisibility(View.GONE);
        this.activeSlot = 0;
    }

    public void showItemInfo(float weight, int img, String title, String text) {
        this.inv_bg_text_item.setVisibility(View.VISIBLE);
        this.weight_item_inv.setVisibility(View.VISIBLE);
        TextView textView = this.weight_item_inv;
        textView.setText(weight + " кг");
        this.img_inv_item.setVisibility(View.VISIBLE);
        this.img_inv_item.setImageResource(img);
        this.text_item_inv_title.setVisibility(View.VISIBLE);
        this.text_item_inv_title.setText(title);
        this.text_item_inv.setVisibility(View.VISIBLE);
        this.text_item_inv.setText(text);
    }

    public void showSlotInfo(int id, int arg) {
        String strTitle = "";
        String strText = "";
        float weight = 0.0f;
        int imgId = 0;
        switch (id) {
            case 1:
                strTitle = "Аптечка";
                strText = "Нужна для лечения";
                imgId = R.drawable.aptech_inv;
                weight = 10.0f;
                break;
            case 2:
                strTitle = "Ремонтный комплект";
                strText = "Нужен для починки своего авто";
                imgId = R.drawable.rem_inv;
                weight = 10.0f;
                break;
            case 3:
                strTitle = "JBL TWS 220";
                strText = "Наушники - позволяют слушать музыку";
                imgId = R.drawable.nau_inv;
                weight = 1.0f;
                break;
            case 4:
                strTitle = "Одежда (id: " + arg + ")";
                strText = "Одежду можно надеть";
                imgId = R.drawable.skin_adm;
                weight = 1.0f;
                break;
            case 5:
                strTitle = "Аксессуар (id: " + arg + ")";
                strText = "Аксессуар - украшения для персонажа";
                imgId = R.drawable.inv_aks;
                weight = 1.0f;
                break;
        }
        if (id != 0) {
            showItemInfo(weight, imgId, strTitle, strText);
        }
    }

    public void addItemToInv(int id, int arg, int slot) {
        int fid = 0;
        switch (id) {
            case 1:
                fid = R.drawable.aptech_inv;
                break;
            case 2:
                fid = R.drawable.rem_inv;
                break;
            case 3:
                fid = R.drawable.nau_inv;
                break;
            case 4:
                fid = R.drawable.skin_adm;
                break;
            case 5:
                fid = R.drawable.inv_aks;
                break;
        }
        if (id == 0 || arg <= 0) {
            switch (slot) {
                case 1:
                    this.img_slot1.setVisibility(View.GONE);
                    this.text_arg1.setVisibility(View.GONE);
                    this.slot1arg = 0;
                    this.slot1ID = 0;
                    clear();
                    return;
                case 2:
                    this.img_slot2.setVisibility(View.GONE);
                    this.text_arg2.setVisibility(View.GONE);
                    this.slot2arg = 0;
                    this.slot2ID = 0;
                    clear();
                    return;
                case 3:
                    this.img_slot3.setVisibility(View.GONE);
                    this.text_arg3.setVisibility(View.GONE);
                    this.slot3arg = 0;
                    this.slot3ID = 0;
                    clear();
                    return;
                case 4:
                    this.img_slot4.setVisibility(View.GONE);
                    this.text_arg4.setVisibility(View.GONE);
                    this.slot4arg = 0;
                    this.slot4ID = 0;
                    clear();
                    return;
                case 5:
                    this.img_slot5.setVisibility(View.GONE);
                    this.text_arg5.setVisibility(View.GONE);
                    this.slot5arg = 0;
                    this.slot5ID = 0;
                    clear();
                    return;
                case 6:
                    this.img_slot6.setVisibility(View.GONE);
                    this.text_arg6.setVisibility(View.GONE);
                    this.slot6arg = 0;
                    this.slot6ID = 0;
                    clear();
                    return;
                case 7:
                    this.img_slot7.setVisibility(View.GONE);
                    this.text_arg7.setVisibility(View.GONE);
                    this.slot7arg = 0;
                    this.slot7ID = 0;
                    clear();
                    return;
                case 8:
                    this.img_slot8.setVisibility(View.GONE);
                    this.text_arg8.setVisibility(View.GONE);
                    this.slot8arg = 0;
                    this.slot8ID = 0;
                    clear();
                    return;
                default:
                    return;
            }
        } else {
            switch (slot) {
                case 1:
                    this.slot1ID = id;
                    this.slot1arg = arg;
                    this.img_slot1.setVisibility(View.VISIBLE);
                    this.img_slot1.setImageResource(fid);
                    this.text_arg1.setVisibility(View.VISIBLE);
                    this.text_arg1.setText(String.valueOf(arg));
                    return;
                case 2:
                    this.slot2ID = id;
                    this.slot2arg = arg;
                    this.img_slot2.setVisibility(View.VISIBLE);
                    this.img_slot2.setImageResource(fid);
                    this.text_arg2.setVisibility(View.VISIBLE);
                    this.text_arg2.setText(String.valueOf(arg));
                    return;
                case 3:
                    this.slot3ID = id;
                    this.slot3arg = arg;
                    this.img_slot3.setVisibility(View.VISIBLE);
                    this.img_slot3.setImageResource(fid);
                    this.text_arg3.setVisibility(View.VISIBLE);
                    this.text_arg3.setText(String.valueOf(arg));
                    return;
                case 4:
                    this.slot4ID = id;
                    this.slot4arg = arg;
                    this.img_slot4.setVisibility(View.VISIBLE);
                    this.img_slot4.setImageResource(fid);
                    this.text_arg4.setVisibility(View.VISIBLE);
                    this.text_arg4.setText(String.valueOf(arg));
                    return;
                case 5:
                    this.slot5ID = id;
                    this.slot5arg = arg;
                    this.img_slot5.setVisibility(View.VISIBLE);
                    this.img_slot5.setImageResource(fid);
                    this.text_arg5.setVisibility(View.VISIBLE);
                    this.text_arg5.setText(String.valueOf(arg));
                    return;
                case 6:
                    this.slot6ID = id;
                    this.slot6arg = arg;
                    this.img_slot6.setVisibility(View.VISIBLE);
                    this.img_slot6.setImageResource(fid);
                    this.text_arg6.setVisibility(View.VISIBLE);
                    this.text_arg6.setText(String.valueOf(arg));
                    return;
                case 7:
                    this.slot7ID = id;
                    this.slot7arg = arg;
                    this.img_slot7.setVisibility(View.VISIBLE);
                    this.img_slot7.setImageResource(fid);
                    this.text_arg7.setVisibility(View.VISIBLE);
                    this.text_arg7.setText(String.valueOf(arg));
                    return;
                case 8:
                    this.slot8ID = id;
                    this.slot8arg = arg;
                    this.img_slot8.setVisibility(View.VISIBLE);
                    this.img_slot8.setImageResource(fid);
                    this.text_arg8.setVisibility(View.VISIBLE);
                    this.text_arg8.setText(String.valueOf(arg));
                    return;
                default:
                    return;
            }
        }
    }
}