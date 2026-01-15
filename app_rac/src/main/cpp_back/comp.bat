@echo off
echo 'Compiling libraries'
powershell -Command "D:\DevTool\NDK\android-ndk-r16b\ndk-build.cmd | Tee-Object -FilePath 'build_log.txt'"
echo Check console output above for details
pause
