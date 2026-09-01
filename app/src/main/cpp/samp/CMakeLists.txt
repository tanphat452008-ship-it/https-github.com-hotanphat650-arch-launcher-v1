cmake_minimum_required(VERSION 3.12)
project(samp)

set(CMAKE_CXX_STANDARD 20)

if(${ANDROID_ABI} STREQUAL "armeabi-v7a")
    add_definitions(-DVER_x32=true)
    set(GLOSSHOOK_ABI "ARM")
elseif(${ANDROID_ABI} STREQUAL "arm64-v8a")
    add_definitions(-DVER_x32=false)
    set(GLOSSHOOK_ABI "ARM64")
endif()

# ดึงไฟล์ .cpp และ .c ทั้งหมดอัตโนมัติ
file(GLOB_RECURSE SOURCES
        "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/*.c"
)

# ตั้งค่าการค้นหาไฟล์ Header (.h)
include_directories(
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_CURRENT_SOURCE_DIR}/gui
        ${CMAKE_CURRENT_SOURCE_DIR}/game/Core
        ${CMAKE_CURRENT_SOURCE_DIR}/game/RW
        ${CMAKE_CURRENT_SOURCE_DIR}/util
        ${CMAKE_CURRENT_SOURCE_DIR}/vendor/shadowhook
        ${CMAKE_CURRENT_SOURCE_DIR}/vendor/imgui
        ${CMAKE_CURRENT_SOURCE_DIR}/vendor
)

add_library(samp SHARED ${SOURCES})

target_link_libraries(samp
        log
        android
        EGL
        GLESv3
        opus
        shadowhook::shadowhook
        ${CMAKE_CURRENT_SOURCE_DIR}/vendor/bass/libs/${ANDROID_ABI}/libbass.so
        ${CMAKE_CURRENT_SOURCE_DIR}/vendor/bass/libs/${ANDROID_ABI}/libbass_ssl.so
        ${CMAKE_CURRENT_SOURCE_DIR}/vendor/GlossHook/libs/${GLOSSHOOK_ABI}/libGlossHook.a
        ${CMAKE_CURRENT_SOURCE_DIR}/vendor/GlossHook/libs/${GLOSSHOOK_ABI}/libGlossHook.so
    
)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -w -s -g -pthread -Wall -O3 -fexceptions")
target_compile_options(samp PRIVATE
        $<$<CONFIG:Debug>:-fvisibility=default>
        $<$<CONFIG:Release>:-fvisibility=hidden>
)
target_link_options(samp PRIVATE "-Wl,-z,max-page-size=16384")
