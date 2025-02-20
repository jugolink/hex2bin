#!/bin/bash

# 进入项目根目录
cd "$(dirname "$0")/.."

# 创建构建目录
mkdir -p build
cd build

# 构建
cmake .. -G "Ninja" -DCMAKE_BUILD_TYPE=Release
cmake --build .

echo "Build completed. Executable can be found in build/bin/" 