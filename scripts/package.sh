#!/bin/bash

# 获取版本号
VERSION=$(grep 'HEX2BIN_VERSION' ../include/version.h.in | cut -d'"' -f2)

# 创建发布目录
mkdir -p release/hex2bin-${VERSION}

# 复制文件
cp ../build/bin/hex2bin* release/hex2bin-${VERSION}/
cp ../README.md release/hex2bin-${VERSION}/
cp ../LICENSE release/hex2bin-${VERSION}/

# 创建压缩包
cd release
if [[ "$OSTYPE" == "msys" ]]; then
    # Windows
    zip -r hex2bin-${VERSION}-win64.zip hex2bin-${VERSION}
else
    # Linux
    tar -czf hex2bin-${VERSION}-linux.tar.gz hex2bin-${VERSION}
fi

# 清理临时文件
rm -rf hex2bin-${VERSION} 