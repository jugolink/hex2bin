#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <filesystem>
#include <numeric>
#include <cstdint>
#include <algorithm>
#include "version.h"

// 字符转16进制数值
uint8_t char_to_hex(char c) {
    if (c >= '0' && c <= '9') {
        return c - '0';
    } else if (c >= 'A' && c <= 'F') {
        return c - 'A' + 10;
    } else if (c >= 'a' && c <= 'f') {
        return c - 'a' + 10;
    }
    return 0;
}

// 将HEX文件的一行转换为字节数组
std::vector<uint8_t> line_to_bytes(const std::string& line) {
    std::vector<uint8_t> bytes;
    for (size_t i = 1; i < line.length() - 1; i += 2) {
        uint8_t high = char_to_hex(line[i]);
        uint8_t low = char_to_hex(line[i + 1]);
        bytes.push_back((high << 4) | low);
    }
    return bytes;
}

// 校验和计算
bool verify_checksum(const std::vector<uint8_t>& line) {
    uint8_t sum = std::accumulate(line.begin(), line.end() - 1, 0);
    return ((0x100 - (sum % 256)) % 256) == line.back();
}

// HEX到BIN的转换函数
bool convert_hex_to_bin(const std::string& hex_path, const std::string& bin_path, bool special_mode = false) {
    std::ifstream hex_file(hex_path);
    if (!hex_file) {
        std::cerr << "Error: Cannot open input file: " << hex_path << std::endl;
        return false;
    }

    std::vector<uint8_t> bin_buffer;
    uint32_t addr_h = 0;
    uint32_t addr_end = 0;

    std::string line;
    while (std::getline(hex_file, line)) {
        if (line.empty() || line[0] != ':') {
            std::cerr << "Error: Illegal format!" << std::endl;
            return false;
        }

        std::vector<uint8_t> bytes = line_to_bytes(line);
        if (!verify_checksum(bytes)) {
            std::cerr << "Error: Checksum failed!" << std::endl;
            return false;
        }

        uint8_t record_type = bytes[3];
        switch (record_type) {
            case 0x00: // 数据记录
            {
                uint16_t addr_l = (bytes[1] << 8) | bytes[2];
                uint8_t length = bytes[0];
                
                for (int i = 4; i < 4 + length; ++i) {
                    bin_buffer.push_back(bytes[i]);
                }
                
                if (length != 0x10) {
                    addr_end = addr_h + addr_l + length - 1;
                }
                break;
            }

            case 0x01: // 文件结束记录
                std::cout << "Hex file successfully resolved\n";
                std::cout << "addr_start: 0x" << std::hex << (addr_end + 1 - bin_buffer.size()) << std::dec << "\n";
                std::cout << "addr_end: 0x" << std::hex << addr_end << std::dec << "\n";
                std::cout << "Total size: " << bin_buffer.size() << " Bytes\n";
                break;

            case 0x02: // 扩展段地址记录
                addr_h = ((bytes[4] << 8) | bytes[5]) << 4;
                break;

            case 0x03: // 开始段地址记录
                // 通常可以忽略
                break;

            case 0x04: // 扩展线性地址记录
                addr_h = (bytes[4] << 24) | (bytes[5] << 16);
                break;

            case 0x05: // 开始线性地址记录
                // 通常可以忽略
                break;

            default:
                std::cerr << "Error: Unknown record type 0x" << std::hex << static_cast<int>(record_type) 
                         << " at line: " << std::dec << line << std::endl;
                return false;
        }
    }

    // 如果输出文件已存在，先删除
    if (std::filesystem::exists(bin_path)) {
        try {
            std::filesystem::remove(bin_path);
            std::cout << "Existing bin file removed: " << bin_path << std::endl;
        } catch (const std::exception& e) {
            std::cerr << "Error while removing existing bin file: " << e.what() << std::endl;
            return false;
        }
    }

    // 写入二进制文件
    std::ofstream bin_file(bin_path, std::ios::binary);
    if (!bin_file) {
        std::cerr << "Error: Cannot create output file: " << bin_path << std::endl;
        return false;
    }

    // 特殊模式：在文件开头添加1024个0字节
    if (special_mode) {
        std::vector<uint8_t> zeros(1024, 0);
        bin_file.write(reinterpret_cast<const char*>(zeros.data()), zeros.size());
    }

    bin_file.write(reinterpret_cast<const char*>(bin_buffer.data()), bin_buffer.size());
    std::cout << "Binary file successfully written to: " << bin_path << std::endl;

    return true;
}

void print_help(const char* program_name) {
    std::cout << "HEX to BIN Converter v" << HEX2BIN_VERSION << " - Command Line Tool\n"
              << "Author: " << HEX2BIN_AUTHOR << "\n"
              << "================================\n\n"
              << "Usage:\n"
              << "  " << program_name << " <input.hex> [output.bin] [special_mode]\n"
              << "  " << program_name << " --help\n"
              << "  " << program_name << " --version\n\n"
              << "Arguments:\n"
              << "  input.hex    Input Intel HEX format file\n"
              << "  output.bin   Output binary file (optional)\n"
              << "              If not specified, output will be saved to Desktop\n"
              << "  special_mode Enable special mode by setting to 'true' (optional)\n"
              << "              Adds 1024 zero bytes at the beginning of output file\n\n"
              << "Examples:\n"
              << "  " << program_name << " input.hex\n"
              << "  " << program_name << " input.hex output.bin\n"
              << "  " << program_name << " input.hex output.bin true\n";
}

void print_version() {
    std::cout << "HEX to BIN Converter v" << HEX2BIN_VERSION << "\n"
              << "Author: " << HEX2BIN_AUTHOR << "\n"
              << "Build Time: " << HEX2BIN_BUILD_TIME << "\n"
              << "Git Commit: " << HEX2BIN_GIT_HASH << "\n";
}

std::string get_desktop_path() {
    #ifdef _WIN32
        return std::string(getenv("USERPROFILE")) + "\\Desktop";
    #else
        return std::string(getenv("HOME")) + "/Desktop";
    #endif
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        print_help(argv[0]);
        return 1;
    }

    std::string arg1 = argv[1];
    if (arg1 == "--help" || arg1 == "-h") {
        print_help(argv[0]);
        return 0;
    } else if (arg1 == "--version" || arg1 == "-v") {
        print_version();
        return 0;
    }

    if (!std::filesystem::exists(arg1)) {
        std::cerr << "Error: The file " << arg1 << " does not exist.\n";
        return 1;
    }

    if (std::filesystem::path(arg1).extension() != ".hex") {
        std::cerr << "Error: The file " << arg1 << " is not a .hex file.\n";
        return 1;
    }

    // 确定输出文件路径
    std::string output_file;
    if (argc > 2) {
        output_file = argv[2];
    } else {
        auto desktop = std::filesystem::path(get_desktop_path());
        output_file = (desktop / std::filesystem::path(arg1).filename().replace_extension(".bin")).string();
    }

    // 检查是否启用特殊模式
    bool special_mode = false;
    if (argc > 3) {
        std::string special_arg = argv[3];
        std::transform(special_arg.begin(), special_arg.end(), special_arg.begin(), ::tolower);
        special_mode = (special_arg == "true");
        if (special_mode) {
            std::cout << "Special Mode Enabled\n";
        }
    }

    try {
        if (!convert_hex_to_bin(arg1, output_file, special_mode)) {
            return 1;
        }
    } catch (const std::exception& e) {
        std::cerr << "Error occurred during the conversion: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}
