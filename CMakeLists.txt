cmake_minimum_required(VERSION 3.10.0)
project(hex2bin VERSION 1.0.0 LANGUAGES CXX)

# 获取 Git 提交哈希
find_package(Git QUIET)
if(GIT_FOUND)
    execute_process(
        COMMAND ${GIT_EXECUTABLE} rev-parse --short HEAD
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
        OUTPUT_VARIABLE GIT_HASH
        OUTPUT_STRIP_TRAILING_WHITESPACE
    )
else()
    set(GIT_HASH "unknown")
endif()

# 获取构建时间戳
string(TIMESTAMP BUILD_TIMESTAMP "%Y-%m-%d %H:%M:%S")

# 从 version.h.in 中提取版本号
file(READ "${PROJECT_SOURCE_DIR}/include/version.h.in" VERSION_CONTENT)
string(REGEX MATCH "HEX2BIN_VERSION \"([0-9]+\.[0-9]+\.[0-9]+)\"" _ ${VERSION_CONTENT})
set(HEX2BIN_VERSION ${CMAKE_MATCH_1})

# 解析版本号为Windows资源文件格式
string(REPLACE "." ";" VERSION_LIST ${HEX2BIN_VERSION})
list(GET VERSION_LIST 0 VER_MAJOR)
list(GET VERSION_LIST 1 VER_MINOR)
list(GET VERSION_LIST 2 VER_PATCH)
set(RC_VERSION "${VER_MAJOR},${VER_MINOR},${VER_PATCH},0")
set(FILE_VERSION "${VER_MAJOR}.${VER_MINOR}.${VER_PATCH}.0")

# 配置版本信息头文件
configure_file(
    "${PROJECT_SOURCE_DIR}/include/version.h.in"
    "${PROJECT_BINARY_DIR}/include/version.h"
    @ONLY
)

# 设置 C++ 标准
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 添加包含目录
include_directories(
    ${PROJECT_SOURCE_DIR}/include
    ${PROJECT_BINARY_DIR}/include
)

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
    OUTPUT_NAME "hex2bin"
)

# 根据平台和编译器设置特定选项
if(WIN32)
    if(MSVC)
        # MSVC 编译器选项
        target_compile_options(hex2bin PRIVATE /W4)
        add_definitions(-D_CRT_SECURE_NO_WARNINGS)
        # Windows - MSVC
        set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
        target_compile_options(hex2bin PRIVATE /MT$<$<CONFIG:Debug>:d>)
    else()
        # MinGW 编译器选项
        target_compile_options(hex2bin PRIVATE -Wall -Wextra)
        # MinGW
        target_link_options(hex2bin PRIVATE -static -static-libgcc -static-libstdc++)
    endif()
else()
    # Linux/Unix 编译器选项
    target_compile_options(hex2bin PRIVATE -Wall -Wextra)
    # Linux
    target_link_options(hex2bin PRIVATE -static-libgcc -static-libstdc++)
endif()

# 添加版本资源（仅Windows）
if(WIN32)
    configure_file(
        "${PROJECT_SOURCE_DIR}/resources/version.rc.in"
        "${PROJECT_BINARY_DIR}/version.rc"
        @ONLY
    )
    if(MSVC)
        target_sources(hex2bin PRIVATE "${PROJECT_BINARY_DIR}/version.rc")
    else()
        # MinGW需要特殊处理
        add_custom_command(
            OUTPUT "${CMAKE_BINARY_DIR}/version.res"
            COMMAND windres.exe
            ARGS
                "--input-format=rc"
                "--output-format=coff"
                "-i" "${PROJECT_BINARY_DIR}/version.rc"
                "-o" "${CMAKE_BINARY_DIR}/version.res"
                "-F" "pe-x86-64"
            DEPENDS "${PROJECT_BINARY_DIR}/version.rc"
            VERBATIM
        )
        target_sources(hex2bin PRIVATE "${CMAKE_BINARY_DIR}/version.res")
    endif()
endif()

# 安装规则
include(GNUInstallDirs)
install(TARGETS hex2bin
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

