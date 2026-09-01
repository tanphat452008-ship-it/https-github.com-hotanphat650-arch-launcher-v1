#include <stdint.h>
#include <stdio.h>
#include <android/log.h>

#define LOG_TAG "SAMP_IMG_UNLOCK"
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO, LOG_TAG, __VA_ARGS__)

// Address สำหรับ v2.10 (GTA:SA Mobile)
#if defined(__arm__) // 32-bit
#define ADDR_ADD_IMG 0x4031B4
    #define ADDR_STREAM_MEM 0x5DE358
#elif defined(__aarch64__) // 64-bit
#define ADDR_ADD_IMG 0x5B5D60
#define ADDR_STREAM_MEM 0x84D838
#endif

extern uintptr_t g_libGTASA;
extern char* g_pszStorage;

void LoadCustomIMGs() {
    if (!g_libGTASA || !g_pszStorage) {
        LOGI("UnlockIMG: libGTASA or g_pszStorage is NULL!");
        return;
    }


    typedef uint32_t (*AddImageToList_t)(const char* szPath, bool bPriority);
    AddImageToList_t AddImageToList = (AddImageToList_t)(g_libGTASA + ADDR_ADD_IMG);

    const char* imgFiles[] = {
            "gta3.img",
            "gta_int.img",
            "player.img",
            "cutscene.img",
            "samp.img"
    };

    char fullPath[512];

    for (int i = 0; i < 5; i++) {
        snprintf(fullPath, sizeof(fullPath), "%s/texdb/%s", g_pszStorage, imgFiles[i]);


        FILE* f = fopen(fullPath, "rb");
        if (f) {
            fclose(f);
            AddImageToList(fullPath, true);
            LOGI("UnlockIMG: Loaded %s", fullPath);
        } else {
            LOGI("UnlockIMG: Skipping %s (Not Found)", fullPath);
        }
    }


    uint32_t* streamingMem = (uint32_t*)(g_libGTASA + ADDR_STREAM_MEM);
    if (streamingMem) {
        *streamingMem = 0x20000000;
        LOGI("UnlockIMG: Streaming Memory set to 512MB");
    }
}