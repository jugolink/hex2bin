@echo off

:: 进入项目根目录
cd /d "%~dp0\.."

:: 创建构建目录
if not exist "build" mkdir build
cd build

:: 构建
cmake .. -G "Ninja" -DCMAKE_BUILD_TYPE=Release
cmake --build .

echo Build completed. Executable can be found in build/bin/

:: 暂停以查看输出
pause 