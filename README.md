# HEX to BIN Converter 🛠️

一个简单而强大的命令行工具，用于将 Intel HEX 格式文件转换为二进制文件。

## ✨ 特性

- 支持标准 Intel HEX 格式文件转换
- 跨平台支持 (Windows 🪟, Linux 🐧)
- 支持特殊模式（在输出文件开头添加1024字节的0）
- 自动校验和检查
- 详细的转换信息输出

## 📋 使用方法

基本用法：
```bash
hex2bin <input.hex> [output.bin] [special_mode]
```

参数说明：
- `input.hex`: 输入的 Intel HEX 格式文件 📄
- `output.bin`: 输出的二进制文件（可选）📦
- `special_mode`: 特殊模式，设置为 "true" 时启用（可选）⚙️

示例：

```bash
基本用法（输出到桌面）
hex2bin input.hex
指定输出文件
hex2bin input.hex output.bin
启用特殊模式
hex2bin input.hex output.bin true
显示帮助信息
hex2bin --help

```

## 🔨 构建指南

### Windows 构建 🪟

#### 环境要求
- MSYS2
- CMake 3.10+
- MinGW-w64 (通过 MSYS2 安装)

#### 安装依赖
```bash
# 在 MSYS2 MINGW64 终端中执行
pacman -Syu
pacman -S mingw-w64-x86_64-cmake mingw-w64-x86_64-ninja mingw-w64-x86_64-gcc
```

#### 构建步骤
```bash
# 在 MSYS2 MINGW64 终端中执行
mkdir build && cd build
cmake .. -G "Ninja" -DCMAKE_BUILD_TYPE=Release
cmake --build .
```

构建完成后，可执行文件位于：`build/bin/hex2bin.exe`

### Linux 构建 🐧

#### 环境要求
- GCC/G++ 支持 C++17
- CMake 3.10+
- Ninja (推荐)

#### 安装依赖
```bash
# Ubuntu/Debian
sudo apt update
sudo apt install build-essential cmake ninja-build

# Fedora
sudo dnf install gcc-c++ cmake ninja-build

# Arch Linux
sudo pacman -S base-devel cmake ninja
```

#### 构建步骤
```bash
mkdir build && cd build
cmake .. -G "Ninja" -DCMAKE_BUILD_TYPE=Release
cmake --build .
```

构建完成后，可执行文件位于：`build/bin/hex2bin`

## 📂 输出文件位置

构建完成后，可执行文件将位于：

- Windows: `build/windows/bin/hex2bin.exe`
- Linux: `build/linux/bin/hex2bin`

## 🔍 注意事项

1. 确保输入文件是有效的 Intel HEX 格式
2. 如果未指定输出文件，程序将在桌面创建同名的 .bin 文件
3. 特殊模式会在输出文件开头添加 1024 个 0 字节
4. 程序会自动进行校验和检查，确保数据完整性

## 📝 许可证

[MIT](LICENSE)

## 🤝 贡献

欢迎提交 Issue 和 Pull Request！


