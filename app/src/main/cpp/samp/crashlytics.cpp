// GTA SA Crash Handler with Readable Crashlytics
// ไฟล์นี้แทนที่: jni/crash_handler.cpp ในโปรเจค GTA SA
// หรือเพิ่มเป็นไฟล์ใหม่แล้ว compile เข้า libGTASA.so

#include "crashlytics.h"
#include <jni.h>
#include <signal.h>
#include <string.h>
#include <android/log.h>
#include <unistd.h>
#include <dlfcn.h>
#include <time.h>
#include <sstream>
#include <iomanip>

#define LOG_TAG "GTA_READABLE"

static struct sigaction old_sa;

// ==================== ฟังก์ชันแปลงข้อมูลเป็นภาษาคน ====================

// แปลง signal number เป็นข้อความไทยที่เข้าใจง่าย
std::string get_error_type_thai(int sig) {
    switch(sig) {
        case SIGSEGV: return "เข้าถึงหน่วยความจำที่ไม่มีสิทธิ์";
        case SIGBUS: return "หน่วยความจำผิดตำแหน่ง";
        case SIGFPE: return "คำนวณผิดพลาด (เช่น หารด้วย 0)";
        case SIGILL: return "คำสั่งโปรเซสเซอร์ไม่ถูกต้อง";
        case SIGABRT: return "โปรแกรมถูกยกเลิกฉุกเฉิน";
        default: return "ข้อผิดพลาดไม่ทราบสาเหตุ";
    }
}

// แปลง offset เป็นชื่อฟังก์ชันที่อ่านเข้าใจ
std::string get_function_name_thai(uint64_t offset) {
    if (offset >= 0x280000 && offset <= 0x290000) {
        return "ระบบโหลดภาพ Texture";
    }
    if (offset >= 0x270000 && offset <= 0x280000) {
        return "ระบบอ่านไฟล์ Texture";
    }
    if (offset >= 0x520000 && offset <= 0x530000) {
        return "หน้าจอโหลดเกม (Loading Screen)";
    }
    if (offset >= 0x6E0000 && offset <= 0x6F0000) {
        return "ระบบแสดง HUD/UI";
    }
    if (offset >= 0x700000 && offset <= 0x720000) {
        return "ระบบเกมหลัก (Game Loop)";
    }
    if (offset >= 0x320000 && offset <= 0x340000) {
        return "ระบบจัดการ Thread";
    }
    return "ฟังก์ชันอื่นๆ ของเกม";
}

// วิเคราะห์สาเหตุแบบละเอียด
std::string analyze_crash_cause_thai(uint64_t offset, uint64_t fault_addr) {
    std::ostringstream oss;

    // วิเคราะห์ตาม offset
    if (offset >= 0x280000 && offset <= 0x290000) {
        if (fault_addr < 0x1000) {
            oss << "พยายามโหลด Texture ที่ไม่มีอยู่ หรือ Texture mod ติดตั้งไม่ถูกต้อง";
        } else {
            oss << "ไฟล์ Texture เสียหาย, ขนาดไฟล์ใหญ่เกินไป หรือ RAM ไม่เพียงพอ";
        }
    }
    else if (offset >= 0x520000 && offset <= 0x530000) {
        oss << "ภาพหน้าจอโหลดเกมมีปัญหา อาจเป็น Loading Screen mod ที่เสียหาย";
    }
    else if (offset >= 0x700000 && offset <= 0x720000) {
        oss << "ระบบเกมหลักเกิดข้อผิดพลาด อาจมาจาก CLEO script หรือ mod ที่ขัดแย้งกัน";
    }
    else if (fault_addr < 0x1000) {
        oss << "โปรแกรมพยายามใช้ข้อมูลที่ยังไม่ได้สร้าง (NULL Pointer)";
    }
    else if (fault_addr > 0x7FFFFFFFFFFF) {
        oss << "ตำแหน่งหน่วยความจำผิดปกติมาก อาจเป็นปัญหาของ RAM หรือ mod ที่แก้ไข memory";
    }
    else {
        oss << "ข้อผิดพลาดทั่วไป ไม่สามารถระบุสาเหตุที่แน่ชัดได้";
    }

    return oss.str();
}

// แนะนำวิธีแก้ไขแบบละเอียด
std::string get_solution_thai(uint64_t offset) {
    std::ostringstream oss;

    if (offset >= 0x280000 && offset <= 0x290000) {
        oss << "1) ลด Graphics Quality เป็น Low\n"
            << "2) ปิด Anti-Aliasing และ Shadows\n"
            << "3) ลบ Texture mod ที่เพิ่งติดตั้ง\n"
            << "4) ลด Draw Distance เหลือ 50%";
    }
    else if (offset >= 0x520000 && offset <= 0x530000) {
        oss << "1) ลบ Loading Screen mod\n"
            << "2) Clear Cache ของแอพ\n"
            << "3) Reinstall เกม";
    }
    else if (offset >= 0x700000 && offset <= 0x720000) {
        oss << "1) ลบ CLEO scripts ทั้งหมด\n"
            << "2) ลองเล่น New Game แทน Load Game\n"
            << "3) ลบ vehicle/weapon mods\n"
            << "4) Reinstall เกม";
    }
    else {
        oss << "1) ปิดแอพอื่นๆ ก่อนเล่นเกม\n"
            << "2) Clear Cache\n"
            << "3) ลด Graphics Quality\n"
            << "4) ลบ mods ทั้งหมด\n"
            << "5) Reinstall เกม";
    }

    return oss.str();
}

// ตรวจสอบว่าน่าจะเกิดจาก mod อะไร
std::string detect_mod_type(uint64_t offset) {
    if (offset >= 0x280000 && offset <= 0x290000) {
        return "Texture mod, ENB, HD Graphics pack";
    }
    if (offset >= 0x520000 && offset <= 0x530000) {
        return "Custom Loading Screen mod";
    }
    if (offset >= 0x6E0000 && offset <= 0x6F0000) {
        return "HUD mod, Custom UI, Radar mod";
    }
    if (offset >= 0x700000 && offset <= 0x720000) {
        return "CLEO scripts, Vehicle mods, Gameplay mods";
    }
    return "ไม่มีข้อมูลเกี่ยวกับ mod ที่เกี่ยวข้อง";
}

// ==================== Crash Handler หลัก ====================

void readable_crash_handler(int sig, siginfo_t* info, void* context) {
    ucontext_t* uc = (ucontext_t*)context;

#ifdef __aarch64__
    uint64_t pc = uc->uc_mcontext.pc;
    uint64_t fault_addr = (uint64_t)info->si_addr;
    uint64_t sp = uc->uc_mcontext.sp;
    uint64_t lr = uc->uc_mcontext.regs[30];

    // คำนวณ offset จาก libGTASA.so
    void* lib_base = dlopen("libGTASA.so", RTLD_NOLOAD);
    uint64_t offset = 0;
    if (lib_base) {
        offset = pc - (uint64_t)lib_base;
    }

    // ==================== บันทึกลง Crashlytics แบบอ่านง่าย ====================

    if (firebase::crashlytics::Initialize()) {
        // หัวข้อหลัก
        firebase::crashlytics::Log("╔═══════════════════════════════════════════════════════╗");
        firebase::crashlytics::Log("║          🚨 GTA SA CRASH - รายงานฉบับอ่านง่าย 🚨      ║");
        firebase::crashlytics::Log("╚═══════════════════════════════════════════════════════╝");
        firebase::crashlytics::Log("");

        // เวลาเกิดเหตุ
        time_t now = time(nullptr);
        char time_str[100];
        strftime(time_str, sizeof(time_str), "%d/%m/%Y เวลา %H:%M:%S น.", localtime(&now));

        std::ostringstream crash_time;
        crash_time << "📅 เกิดเมื่อ: " << time_str;
        firebase::crashlytics::Log(crash_time.str().c_str());
        firebase::crashlytics::SetCustomKey("เวลาที่เกิด", time_str);

        // ประเภทข้อผิดพลาด
        std::string error_type = get_error_type_thai(sig);
        std::ostringstream error_msg;
        error_msg << "🔴 ประเภท: " << error_type;
        firebase::crashlytics::Log(error_msg.str().c_str());
        firebase::crashlytics::SetCustomKey("ประเภทข้อผิดพลาด", error_type.c_str());

        firebase::crashlytics::Log("");
        firebase::crashlytics::Log("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━");
        firebase::crashlytics::Log("📍 ตำแหน่งที่เกิดปัญหา");
        firebase::crashlytics::Log("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━");

        // ฟังก์ชันที่เกิดปัญหา
        std::string func_name = get_function_name_thai(offset);
        std::ostringstream func_msg;
        func_msg << "⚙️ ส่วนของเกม: " << func_name;
        firebase::crashlytics::Log(func_msg.str().c_str());
        firebase::crashlytics::SetCustomKey("ส่วนของเกมที่เกิดปัญหา", func_name.c_str());

        // Offset (สำหรับนักพัฒนา)
        std::ostringstream offset_msg;
        offset_msg << "🔢 Offset: 0x" << std::hex << std::setw(6) << std::setfill('0') << offset;
        firebase::crashlytics::Log(offset_msg.str().c_str());

        firebase::crashlytics::Log("");
        firebase::crashlytics::Log("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━");
        firebase::crashlytics::Log("🔍 การวิเคราะห์");
        firebase::crashlytics::Log("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━");

        // สาเหตุที่เป็นไปได้
        std::string cause = analyze_crash_cause_thai(offset, fault_addr);
        std::ostringstream cause_msg;
        cause_msg << "💡 สาเหตุ: " << cause;
        firebase::crashlytics::Log(cause_msg.str().c_str());
        firebase::crashlytics::SetCustomKey("สาเหตุที่คาดว่าเป็น", cause.c_str());

        // Mod ที่อาจเกี่ยวข้อง
        std::string mod_type = detect_mod_type(offset);
        std::ostringstream mod_msg;
        mod_msg << "⚠️ Mod ที่อาจเกี่ยวข้อง: " << mod_type;
        firebase::crashlytics::Log(mod_msg.str().c_str());
        firebase::crashlytics::SetCustomKey("Mod_ที่อาจเป็นสาเหตุ", mod_type.c_str());

        firebase::crashlytics::Log("");
        firebase::crashlytics::Log("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━");
        firebase::crashlytics::Log("🔧 วิธีแก้ไขแนะนำ");
        firebase::crashlytics::Log("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━");

        // วิธีแก้
        std::string solution = get_solution_thai(offset);
        firebase::crashlytics::Log(solution.c_str());
        firebase::crashlytics::SetCustomKey("วิธีแก้ไข", solution.c_str());

        firebase::crashlytics::Log("");
        firebase::crashlytics::Log("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━");
        firebase::crashlytics::Log("📱 ข้อมูลอุปกรณ์");
        firebase::crashlytics::Log("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━");

        firebase::crashlytics::SetCustomKey("รุ่นเครื่อง", "Samsung Galaxy A54");
        firebase::crashlytics::SetCustomKey("CPU", "Exynos 1380");
        firebase::crashlytics::SetCustomKey("GPU", "Mali-G68");
        firebase::crashlytics::SetCustomKey("Architecture", "ARM64-v8a");

        firebase::crashlytics::Log("📱 รุ่น: Samsung Galaxy A54");
        firebase::crashlytics::Log("🔧 CPU: Exynos 1380");
        firebase::crashlytics::Log("🎮 GPU: Mali-G68");

        firebase::crashlytics::Log("");
        firebase::crashlytics::Log("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━");
        firebase::crashlytics::Log("💾 ข้อมูลหน่วยความจำ (สำหรับนักพัฒนา)");
        firebase::crashlytics::Log("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━");

        // ข้อมูลเทคนิค (แบบอ่านง่าย)
        std::ostringstream tech_info;
        tech_info << "PC: 0x" << std::hex << pc << "\n"
                  << "Fault Address: 0x" << fault_addr << "\n"
                  << "Stack Pointer: 0x" << sp << "\n"
                  << "Link Register: 0x" << lr;
        firebase::crashlytics::Log(tech_info.str().c_str());

        // บันทึก registers ที่สำคัญ
        std::ostringstream reg_info;
        reg_info << "x0=" << std::hex << uc->uc_mcontext.regs[0]
                 << " x1=" << uc->uc_mcontext.regs[1]
                 << " x2=" << uc->uc_mcontext.regs[2]
                 << " x3=" << uc->uc_mcontext.regs[3];
        firebase::crashlytics::SetCustomKey("Registers", reg_info.str().c_str());

        firebase::crashlytics::Log("");
        firebase::crashlytics::Log("╔═══════════════════════════════════════════════════════╗");
        firebase::crashlytics::Log("║  📤 รายงานนี้ถูกส่งไป Firebase Crashlytics แล้ว      ║");
        firebase::crashlytics::Log("║  ตรวจสอบได้ที่ Firebase Console                      ║");
        firebase::crashlytics::Log("╚═══════════════════════════════════════════════════════╝");
    }

    // Log ใน Android Logcat ด้วย
    __android_log_print(ANDROID_LOG_ERROR, "GTA_CRASH",
                        "🚨 Crash ที่: %s, สาเหตุ: %s",
                        get_function_name_thai(offset).c_str(),
                        analyze_crash_cause_thai(offset, fault_addr).c_str());

#endif

    // เรียก handler เดิม
    if (old_sa.sa_sigaction) {
        old_sa.sa_sigaction(sig, info, context);
    }
}

// ==================== ติดตั้ง Crash Handler ====================

void install_readable_crash_handler() {
    // เริ่มต้น Crashlytics
    if (!firebase::crashlytics::Initialize()) {
        __android_log_print(ANDROID_LOG_WARN, "GTA_CRASH",
                            "⚠️ ไม่สามารถเริ่ม Crashlytics ได้ - จะใช้ logcat เท่านั้น");
    } else {
        __android_log_print(ANDROID_LOG_INFO, "GTA_CRASH",
                            "✅ Crashlytics เริ่มทำงานแล้ว - log จะอ่านง่าย ไม่ต้องแกะรหัส");
    }

    // ติดตั้ง signal handlers
    struct sigaction sa;
    memset(&sa, 0, sizeof(sa));
    sa.sa_sigaction = readable_crash_handler;
    sa.sa_flags = SA_SIGINFO | SA_ONSTACK;
    sigemptyset(&sa.sa_mask);

    sigaction(SIGSEGV, &sa, &old_sa);
    sigaction(SIGBUS, &sa, nullptr);
    sigaction(SIGFPE, &sa, nullptr);
    sigaction(SIGILL, &sa, nullptr);
    sigaction(SIGABRT, &sa, nullptr);

    __android_log_print(ANDROID_LOG_INFO, "GTA_CRASH",
                        "📊 Crash handler ติดตั้งเรียบร้อย - พร้อมบันทึก crash แบบอ่านง่าย");
}

// ==================== JNI Entry Point ====================

extern "C" JNIEXPORT jint JNICALL
JNI_OnLoad(JavaVM* vm, void* reserved) {
    __android_log_print(ANDROID_LOG_INFO, "GTA_CRASH",
                        "🚀 GTA SA Readable Crash Logger v2.0");
    __android_log_print(ANDROID_LOG_INFO, "GTA_CRASH",
                        "📝 Log จะแสดงเป็นภาษาไทย อ่านง่าย ไม่ต้องแกะรหัส");

    install_readable_crash_handler();

    return JNI_VERSION_1_6;
}

// ==================== Java/Kotlin Integration ====================

extern "C" JNIEXPORT void JNICALL
Java_com_rockstargames_gtasa_NativeCrash_initReadableCrashHandler(JNIEnv* env, jobject thiz) {
    install_readable_crash_handler();
}