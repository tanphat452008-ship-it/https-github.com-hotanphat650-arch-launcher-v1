#include "../main.h"
#include "../game/game.h"
#include "../vendor/shadowhook/patch.h"
#include "vehicleColoursTable.h"
#include "../settings.h"
#include "game.h"
#include "World.h"
#include "net/netgame.h"

extern CSettings* pSettings;
extern CGame* pGame;

// จองหน่วยความจำให้กว้างขึ้นและปลอดภัย
VehicleAudioPropertiesStruct VehicleAudioProperties[20000];

// --- [ฟังก์ชันโหลดการตั้งค่าเสียงรถ] ---
void readVehiclesAudioSettings()
{
    char vehicleModel[50];
    int16_t pIndex = 0;
    FILE* pFile;
    char line[300];

    memset(VehicleAudioProperties, 0x00, sizeof(VehicleAudioProperties));
    VehicleAudioPropertiesStruct Current;

    char buffer[0xFF];
    snprintf(buffer, sizeof(buffer), "%sSAMP/vehicleAudioSettings.cfg", g_pszStorage);

    pFile = fopen(buffer, "r");
    if (!pFile) return;

    while (fgets(line, sizeof(line), pFile))
    {
        if (line[0] == ';' || line[0] == '\n' || line[0] == '\r') continue;
        if (strncmp(line, ";the end", 8) == 0) break;

        // ดึงข้อมูล 14 parameters
        int count = sscanf(line, "%s %d %d %d %d %f %f %d %f %d %d %d %d %f",
                           vehicleModel, &Current.VehicleType, &Current.EngineOnSound,
                           &Current.EngineOffSound, &Current.field_4, &Current.field_5,
                           &Current.field_6, &Current.HornTon, &Current.HornHigh,
                           &Current.DoorSound, &Current.RadioNum, &Current.RadioType,
                           &Current.field_14, &Current.field_16);

        if(count < 14) continue;

        // รับ Index ของรถจากตัวเกม
        ((void (*)(const char*, int16_t*))(g_libGTASA + 0x385E38 + 1))(vehicleModel, &pIndex);

        // ป้องกัน Buffer Overflow (สาเหตุหลักที่ทำให้บางรุ่นเด้ง)
        int finalIdx = pIndex - 400;
        if (finalIdx >= 0 && finalIdx < 20000) {
            memcpy(&VehicleAudioProperties[finalIdx], &Current, sizeof(VehicleAudioPropertiesStruct));
        }
    }
    fclose(pFile);
}

// --- [Patch จำกัด FPS] ---
void ApplyFPSPatch()
{
    uint8_t fps = pSettings->Get().iFPSCount;
    if(fps <= 0) fps = 30;

#if VER_x32
    CHook::WriteMemory(g_libGTASA + 0x005E49E0, (uintptr_t)&fps, 1);
    CHook::WriteMemory(g_libGTASA + 0x005E492E, (uintptr_t)&fps, 1);
#else
    CHook::NOP(g_libGTASA + 0x70A474, 1);
    CHook::NOP(g_libGTASA + 0x70A398, 1);

    pGame->PostToMainThread([=] {
        auto RsGlobal = (RsGlobalType*)(g_libGTASA + 0xC9B320);
        CHook::UnFuck(g_libGTASA + 0xC9B320);
        RsGlobal->maxFPS = fps;
    });
#endif
}

// --- [Patch ปิดระบบ Auto Aim] ---
void DisableAutoAim()
{
    CHook::RET("_ZN10CPlayerPed22FindWeaponLockOnTargetEv");
    CHook::RET("_ZN10CPlayerPed26FindNextWeaponLockOnTargetEP7CEntityb");
    CHook::RET("_ZN4CPed21SetWeaponLockOnTargetEP7CEntity");
}

// --- [Patch ระหว่างการเล่นเกม] ---
void ApplyInGamePatches()
{
    // ปลดล็อคแผนที่ทั้งหมด
    memset((void*)(g_libGTASA + (VER_x32 ? 0x0098D252 : 0xC1BF92)), 1, 100);
    *(uint32_t*)(g_libGTASA + (VER_x32 ? 0x0098D2B8 : 0xC1BFF8)) = 100;

    // CPlayerPed Fix สำหรับ 32/64 bit
#if VER_x32
    CHook::WriteMemory(g_libGTASA + 0x004C36E2, (uintptr_t)"\xE0", 1);
#else
    CHook::WriteMemory(g_libGTASA + 0x5C0BC4, (uintptr_t)"\x34\x00\x80\x52", 4);
#endif

    // ปิดเสียงวิทยุและเสียงรถที่ไม่ได้ใช้งานในออนไลน์
    CHook::RET("_ZN20CAERadioTrackManager7ServiceEi");
    CHook::RET("_ZN4CPed31RemoveWeaponWhenEnteringVehicleEi");

    // NOP ระบบ Audio ที่ทำให้เครื่องโหลดหนัก
    CHook::NOP(g_libGTASA + (VER_x32 ? 0x00553E96 : 0x674610), 2);
    CHook::NOP(g_libGTASA + (VER_x32 ? 0x00561AC2 : 0x682C1C), 2);
}

// --- [Patch ระบบพื้นฐาน (Level 0)] ---
void ApplyPatches_level0()
{
    CHook::Write(g_libGTASA + (VER_x32 ? 0x006783C0 : 0x84E7A8), &CWorld::Players);

    // Fix Alpha Raster (แก้ปัญหาภาพบัคในบาง GPU)
#if VER_x32
    CHook::WriteMemory(g_libGTASA + 0x001AE8DE, (uintptr_t)"\x01\x22", 2);
#else
    CHook::WriteMemory(g_libGTASA + 0x23FDE0, (uintptr_t)"\x22\x00\x80\x52", 4);
#endif

    // ปิดระบบกระจกและเงา (ช่วยให้มือถือรุ่นสเปคต่ำเล่นได้)
    CHook::RET("_ZN22CRealTimeShadowManager4InitEv");
    CHook::RET("_ZN8CMirrors16RenderReflBufferEb");
    CHook::RET("_ZN6CGlass4InitEv");
    CHook::RET("_ZN5CHeli9InitHelisEv");
    CHook::RET("_ZN11CPopulation10InitialiseEv");
}

// --- [Patch หลักในการแก้ Crash และเพิ่มประสิทธิภาพ] ---
void ApplyPatches()
{
    // ปิด Social Club Config (ลดอาการค้างตอนเข้าเกม)
    CHook::NOP(g_libGTASA + (VER_x32 ? 0x2A4A62 : 0x3634A4), 1);

    // แก้ปัญหาป้ายทะเบียนรถทำเครื่องค้าง
    CHook::RET("_ZN17CVehicleModelInfo17SetCarCustomPlateEv");

    // แก้ไขชื่อไฟล์เซฟ (สำคัญสำหรับ Android 11+)
    char* DefaultPCSaveFileName = (char*)(g_libGTASA + (VER_x32 ? 0x006B012C : 0x88CB08));
    if(DefaultPCSaveFileName) memcpy(DefaultPCSaveFileName, "GTASAMP", 8);

    // ปิด Sun Reflection ที่มักจะ Crash ในชิป Adreno
#if VER_x32
    CHook::NOP(g_libGTASA + 0x003F61B6, 2);
#else
    CHook::NOP(g_libGTASA + 0x004D8700, 1);
#endif

    // ปิดระบบที่ไม่จำเป็นใน SAMP เพื่อลดการทำงานของ CPU
    CHook::RET("_ZN18CMotionBlurStreaks6UpdateEv");
    CHook::RET("_ZN11CPlayerInfo17FindObjectToStealEP4CPed");
    CHook::RET("_ZN7CCamera8CamShakeEffff");
    CHook::RET("_ZN6CRadar10DrawLegendEiii");

    // บังคับให้ Streaming Memory ว่างเสมอ (แก้ปัญหา Texture หาย)
    CHook::NOP(g_libGTASA + (VER_x32 ? 0x0046BE88 : 0x55774C), 1);

    // Fix ปัญหานักเลง/NPC เกิดแล้วเกมหลุด
    CHook::RET("_ZN10CPedGroups7ProcessEv");
    CHook::RET("_ZN11CPopulation15AddToPopulationEffff");
}

void InstallVehicleEngineLightPatches() {
    // ฟังก์ชันเพิ่มเติมสำหรับไฟรถ (ถ้าจำเป็น)
}