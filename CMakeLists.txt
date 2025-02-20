cmake_minimum_required(VERSION 3.10.0)
project(hex2bin VERSION 0.1.0 LANGUAGES CXX)

# 设置 C++ 标准
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 设置平台特定的输出名称
if(WIN32)
    set(OUTPUT_SUFFIX ".exe")
else()
    set(OUTPUT_SUFFIX "")
endif()

# 添加可执行文件
add_executable(hex2bin main.cpp)

# 设置输出目录和名称
set_target_properties(hex2bin PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
    RUNTIME_OUTPUT_DIRECTORY_RELEASE "${CMAKE_BINARY_DIR}/bin"
    OUTPUT_NAME "hex2bin${OUTPUT_SUFFIX}"
)

# 根据平台和编译器设置特定选项
if(WIN32)
    if(MSVC)
        # MSVC 编译器选项
        target_compile_options(hex2bin PRIVATE /W4)
        add_definitions(-D_CRT_SECURE_NO_WARNINGS)
    else()
        # MinGW 编译器选项
        target_compile_options(hex2bin PRIVATE -Wall -Wextra)
    endif()
else()
    # Linux/Unix 编译器选项
    target_compile_options(hex2bin PRIVATE -Wall -Wextra)
endif()

# 安装规则
include(GNUInstallDirs)
install(TARGETS hex2bin
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

