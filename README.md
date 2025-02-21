# HEX to BIN Converter 🛠️

一个简单而强大的命令行工具，用于将 Intel HEX 格式文件转换为二进制文件。

## 📘 文件格式说明

### Intel HEX 格式 (.hex)
Intel HEX 是一种用于编程的文本格式文件，常用于微控制器编程。每行记录包含：
- 起始符 ':'
- 数据长度
- 地址信息
- 记录类型
- 数据
- 校验和

示例行：
```
:10246200464C5549442050524F46494C4500464C33
```
- :     → 起始标记
- 10    → 16字节数据
- 2462  → 地址
- 00    → 数据记录
- 464C...→ 实际数据
- 33    → 校验和

### 二进制格式 (.bin)
BIN 文件是纯二进制格式，直接包含程序代码和数据，没有额外的地址信息或校验和。它是内存的直接映像。

## 🔄 转换原理

程序将 HEX 文件转换为 BIN 文件的过程：

### 1. HEX 文件记录类型说明

#### 数据记录 (0x00)
```
:10246200464C5549442050524F46494C4500464C33
```
- :     → 起始标记
- 10    → 16字节数据
- 2462  → 地址
- 00    → 数据记录类型
- 464C...→ 实际数据
- 33    → 校验和

#### 文件结束记录 (0x01)
```
:00000001FF
```
- :     → 起始标记
- 00    → 0字节数据
- 0000  → 地址（通常为0）
- 01    → 结束记录类型
- FF    → 校验和

#### 扩展段地址记录 (0x02)
```
:02000002FFFF00
```
- :     → 起始标记
- 02    → 2字节数据
- 0000  → 地址（通常为0）
- 02    → 扩展段地址类型
- FFFF  → 段地址（左移4位后与数据地址相加）
- 00    → 校验和

#### 开始段地址记录 (0x03)
```
:04000003000000F5
```
- :     → 起始标记
- 04    → 4字节数据
- 0000  → 地址（通常为0）
- 03    → 开始段地址类型
- 0000  → CS值
- 00F5  → IP值
- 00    → 校验和

#### 扩展线性地址记录 (0x04)
```
:020000040800F2
```
- :     → 起始标记
- 02    → 2字节数据
- 0000  → 地址（通常为0）
- 04    → 扩展线性地址类型
- 0800  → 高16位地址（左移16位后与数据地址相加）
- F2    → 校验和

#### 开始线性地址记录 (0x05)
```
:04000005080000FF34
```
- :     → 起始标记
- 04    → 4字节数据
- 0000  → 地址（通常为0）
- 05    → 开始线性地址类型
- 08000000 → EIP值（程序入口点）
- 34    → 校验和

### 2. 地址计算
- 对于数据记录 (0x00)：最终地址 = 扩展地址 + 记录中的地址
  * 使用扩展段地址 (0x02)：地址 = (扩展段地址 << 4) + 记录地址
  * 使用扩展线性地址 (0x04)：地址 = (扩展线性地址 << 16) + 记录地址

### 3. 数据处理
- 按地址顺序组织数据
- 提取数据记录中的实际数据
- 处理地址连续性
- 可选：添加特殊模式的1024字节填充

### 4. 生成二进制文件
- 创建新的二进制文件
- 按顺序写入数据
- 确保数据完整性

## ✨ 特性

- 支持标准 Intel HEX 格式文件转换
- 跨平台支持 (Windows 🪟, Linux 🐧)
- 支持特殊模式（在输出文件开头添加1024字节的0）
- 自动校验和检查
- 详细的转换信息输出
- 版本信息和构建信息追踪
- 自动文件大小单位换算（B/KB/MB）
- 可选的详细地址信息显示

## 📋 使用方法

基本用法：
```bash
hex2bin <input.hex> [output.bin] [special_mode] [verbose]
```

参数说明：
- `input.hex`: 输入的 Intel HEX 格式文件 📄
- `output.bin`: 输出的二进制文件（可选）📦
- `special_mode`: 特殊模式，设置为 "true" 时启用（可选）⚙️
- `verbose`: 显示详细地址信息，设置为 "verbose" 时启用（可选）🔍
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

# 启用特殊模式和详细信息
hex2bin input.hex output.bin true verbose
```

输出示例：
```bash
Hex file successfully resolved
Total size: 2048 Bytes (2.00 KB)
Binary file successfully written to: output.bin

# 启用详细模式时的输出
Hex file successfully resolved
Debug info:
  addr_start: 0x1000
  addr_end: 0x17FF
Total size: 2048 Bytes (2.00 KB)
Binary file successfully written to: output.bin
```

版本信息输出示例：
```bash
HEX to BIN Converter v1.0.0
Author: XXX
Build Time: 2024-02-20 15:30:45
Git Commit: abc1234
```

## 🔍 详细模式说明

当使用 verbose 参数时，程序会显示以下额外信息：

### 地址信息
- `addr_start`: 数据在目标设备中的起始地址
- `addr_end`: 数据在目标设备中的结束地址

这些地址信息对于以下场景特别有用：
- 微控制器程序开发
- 需要确认程序加载位置
- 验证程序是否在正确的内存区域

示例输出：
```bash
Hex file successfully resolved
Debug info:
  addr_start: 0x1000  # 程序起始于地址 0x1000
  addr_end: 0x17FF    # 程序结束于地址 0x17FF
Total size: 2048 Bytes (2.00 KB)
Binary file successfully written to: output.bin
```

注意：这些地址信息来自原始的 Intel HEX 文件，表示数据在目标设备中的实际加载位置。对于普通的二进制文件转换，这些信息不是必需的。

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



