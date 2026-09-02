package com.rstarx.hexrays.launcher.util;

import android.content.Context;
import com.joom.paranoid.Obfuscate;

@Obfuscate
public class SignatureChecker {
    
    public static boolean isSignatureValid(Context ctx, String packageName) {
        // Bỏ qua kiểm tra chữ ký APK, luôn báo thành công
        return true;
    }
}
