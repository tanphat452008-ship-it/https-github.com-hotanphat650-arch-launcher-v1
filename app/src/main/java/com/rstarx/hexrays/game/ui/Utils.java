package com.rstarx.hexrays.game.ui;

import android.animation.Animator;
import android.animation.AnimatorListenerAdapter;
import android.app.Activity;
import android.graphics.drawable.Drawable;
import android.text.Html;
import android.text.Spanned;
import android.view.View;
import androidx.appcompat.app.AppCompatActivity;
import androidx.core.content.ContextCompat;
import java.security.SecureRandom;
import java.util.ArrayList;
import java.util.Iterator;
import java.util.LinkedList;
import java.util.regex.Pattern;
import org.apache.commons.io.IOUtils;

/* loaded from: classes12.dex */
public class Utils {
    static final String AB = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
    public static long ANIMATION_DELAY;
    public static AppCompatActivity currentContext;
    static SecureRandom rnd = new SecureRandom();

    public static void ShowLayout(View view, boolean isAnim) {
        if (view != null) {
            view.setVisibility(0);
            if (isAnim) {
                fadeIn(view);
            } else {
                view.setAlpha(1.0f);
            }
        }
    }

    public static void HideLayout(View view, boolean isAnim) {
        if (view != null) {
            if (isAnim) {
                fadeOut(view);
            } else {
                view.setAlpha(0.0f);
                view.setVisibility(8);
            }
        }
    }

    public static void ShowDialogLayout(View view, boolean isAnim) {
        if (view != null) {
            view.setVisibility(0);
            if (isAnim) {
                fadeInDialog(view);
            } else {
                view.setAlpha(1.0f);
            }
        }
    }

    public static void HideDialogLayout(View view, boolean isAnim) {
        if (view != null) {
            if (isAnim) {
                fadeOutDialog(view);
            } else {
                view.setAlpha(0.0f);
                view.setVisibility(8);
            }
        }
    }

    private static void fadeIn(View view) {
        if (view != null) {
            view.animate().setDuration(1000L).setListener(new AnimatorListenerAdapter() { // from class: com.legend.online.gui.util.Utils.1
                @Override // android.animation.AnimatorListenerAdapter, android.animation.Animator.AnimatorListener
                public void onAnimationEnd(Animator animation) {
                    super.onAnimationEnd(animation);
                }
            }).alpha(1.0f);
        }
    }

    private static void fadeOut(final View view) {
        if (view != null) {
            view.animate().setDuration(1000L).setListener(new AnimatorListenerAdapter() { // from class: com.legend.online.gui.util.Utils.2
                @Override // android.animation.AnimatorListenerAdapter, android.animation.Animator.AnimatorListener
                public void onAnimationEnd(Animator animation) {
                    view.setVisibility(8);
                    super.onAnimationEnd(animation);
                }
            }).alpha(0.0f);
        }
    }

    private static void fadeInDialog(View view) {
        if (view != null) {
            view.animate().setDuration(150L).setListener(new AnimatorListenerAdapter() { // from class: com.legend.online.gui.util.Utils.3
                @Override // android.animation.AnimatorListenerAdapter, android.animation.Animator.AnimatorListener
                public void onAnimationEnd(Animator animation) {
                    super.onAnimationEnd(animation);
                }
            }).alpha(1.0f);
        }
    }

    private static void fadeOutDialog(final View view) {
        if (view != null) {
            view.animate().setDuration(150L).setListener(new AnimatorListenerAdapter() { // from class: com.legend.online.gui.util.Utils.4
                @Override // android.animation.AnimatorListenerAdapter, android.animation.Animator.AnimatorListener
                public void onAnimationEnd(Animator animation) {
                    view.setVisibility(8);
                    super.onAnimationEnd(animation);
                }
            }).alpha(0.0f);
        }
    }

    public static Drawable getRes(Activity activity, int id) {
        return ContextCompat.getDrawable(activity.getApplicationContext(), id);
    }

    public static Spanned transfromColors(String str) {
        int i;
        LinkedList linkedList = new LinkedList();
        int i2 = 0;
        String str2 = str;
        int i3 = 0;
        for (int i4 = 0; i4 < str2.length(); i4++) {
            if (str2.charAt(i4) == '{' && (i = i4 + 7) < str2.length()) {
                int i5 = i4 + 1;
                String sb = "#" + str2.substring(i5, i);
                linkedList.addLast(sb);
                str2 = str2.substring(0, i5) + "repl" + i3 + str2.substring(i);
                i3++;
            }
        }
        Iterator it = linkedList.iterator();
        while (it.hasNext()) {
            String str3 = (String) it.next();
            str2 = i2 == 0 ? str2.replaceAll(Pattern.quote("{repl" + i2 + "}"), "<font color='" + str3 + "'>") : str2.replaceAll(Pattern.quote("{repl" + i2 + "}"), "</font><font color='" + str3 + "'>");
            i2++;
        }
        if (linkedList.size() >= 1) {
            str2 = str2 + "</font>";
        }
        return Html.fromHtml(str2.replaceAll(Pattern.quote(IOUtils.LINE_SEPARATOR_UNIX), "<br>"));
    }

    public static ArrayList<String> fixFieldsForDialog(ArrayList<String> fields) {
        ArrayList<String> newList = new ArrayList<>();
        int max = 0;
        for (int i = 0; i < fields.size(); i++) {
            int len = fields.get(i).split("\t").length;
            if (len > max) {
                max = len;
            }
        }
        for (int i2 = 0; i2 < fields.size(); i2++) {
            StringBuilder field = new StringBuilder(fields.get(i2));
            for (int len2 = fields.get(i2).split("\t").length; len2 != max; len2++) {
                field.append("\\t ");
            }
            newList.add(field.toString());
        }
        return newList;
    }

    public static String randomString(int len) {
        StringBuilder sb = new StringBuilder(len);
        for (int i = 0; i < len; i++) {
            sb.append(AB.charAt(rnd.nextInt(AB.length())));
        }
        return sb.toString();
    }
}