# HEX to BIN Converter 🛠️

一个简单而强大的命令行工具，用于将 Intel HEX 格式文件转换为二进制文件。

## ✨ 特性

- 支持标准 Intel HEX 格式文件转换
- 跨平台支持 (Windows 🪟, Linux 🐧)
- 支持特殊模式（在输出文件开头添加1024字节的0）
- 自动校验和检查
- 详细的转换信息输出
- 版本信息和构建信息追踪

## 📋 使用方法

基本用法：
```bash
hex2bin <input.hex> [output.bin] [special_mode]
```

参数说明：
- `input.hex`: 输入的 Intel HEX 格式文件 📄
- `output.bin`: 输出的二进制文件（可选）📦
- `special_mode`: 特殊模式，设置为 "true" 时启用（可选）⚙️
- `--help` 或 `-h`: 显示帮助信息
- `--version` 或 `-v`: 显示版本信息

示例：
```bash
# 基本用法（输出到桌面）
hex2bin input.hex

# 指定输出文件
hex2bin input.hex output.bin

# 启用特殊模式
hex2bin input.hex output.bin true

# 显示帮助信息
hex2bin --help

# 显示版本信息
hex2bin --version
```

版本信息输出示例：
```bash
HEX to BIN Converter v1.0.0
Author: XXX
Build Time: 2024-02-20 15:30:45
Git Commit: abc1234
```

## 🔨 构建指南

### Windows 构建 🪟

#### 方法1：使用预编译的可执行文件
从 Releases 页面下载最新的发布版本，包含所有必要的依赖。

#### 方法2：从源码构建（静态链接）

```bash
# 在 MSYS2 MINGW64 终端中执行
mkdir build && cd build
cmake .. -G "Ninja" -DCMAKE_BUILD_TYPE=Release
cmake --build .
```

### Linux 构建 🐧

#### 方法1：使用预编译的可执行文件
从 Releases 页面下载最新的发布版本，包含所有必要的依赖。

#### 方法2：从源码构建（静态链接）

```bash
mkdir build && cd build
cmake .. -G "Ninja" -DCMAKE_BUILD_TYPE=Release
cmake --build .
```

## 📦 发布包内容

预编译的发布包包含：
- hex2bin 可执行文件（静态链接，无需额外依赖）
- README.md
- LICENSE
- 示例文件（可选）

## 🔍 注意事项

1. 预编译的可执行文件已经静态链接了所有必要的依赖
2. 无需安装额外的运行时库
3. Windows版本包含完整的版本信息和文件属性

## 📝 许可证

[MIT](LICENSE)

## 🤝 贡献

欢迎提交 Issue 和 Pull Request！


