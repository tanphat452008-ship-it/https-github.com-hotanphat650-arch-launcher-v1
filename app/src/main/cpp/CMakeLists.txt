cmake_minimum_required(VERSION 3.12)
project(reversed)

include_directories(
        ${CMAKE_CURRENT_LIST_DIR}/opus/include
)

find_package(shadowhook REQUIRED CONFIG)

# Thêm dòng này để tìm module Firebase qua Prefab
find_package(firebase-crashlytics-ndk REQUIRED CONFIG)

add_subdirectory(opus)
add_subdirectory(samp)

target_compile_options(opus PRIVATE -O3)
