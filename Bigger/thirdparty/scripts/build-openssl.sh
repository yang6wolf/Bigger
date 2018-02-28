
OPENSSL_SRC_DIR=~/Documents/openssl
OPENSSL_BUILD_DIR=${PWD}/openssl-build
OPENSSL_BUILD_LOG_DIR=${OPENSSL_BUILD_DIR}/log
OPENSSL_BUILD_UNIVERSAL_DIR=${OPENSSL_BUILD_DIR}/universal
OPENSSL_UNIVERSAL_LIB_DIR=${OPENSSL_BUILD_UNIVERSAL_DIR}/lib

set -u

rm -rf ${OPENSSL_BUILD_DIR}
if [ ! -d "${OPENSSL_BUILD_UNIVERSAL_DIR}" ]; then
    mkdir -p "${OPENSSL_BUILD_UNIVERSAL_DIR}"
fi

if [ ! -d "${OPENSSL_BUILD_LOG_DIR}" ]; then
    mkdir "${OPENSSL_BUILD_LOG_DIR}"
fi

if [ ! -d "${OPENSSL_UNIVERSAL_LIB_DIR}" ]; then
    mkdir "${OPENSSL_UNIVERSAL_LIB_DIR}"
fi

pushd .
cd ${OPENSSL_SRC_DIR}

#@CLANG=$(xcrun --find clang)

IPHONE_OS_SDK_PATH=$(xcrun -sdk iphoneos --show-sdk-path)
IPHONE_OS_CROSS_TOP=${IPHONE_OS_SDK_PATH//\/SDKs*/}
IPHONE_OS_CROSS_SDK=${IPHONE_OS_SDK_PATH##*/}

IPHONE_SIMULATOR_SDK_PATH=$(xcrun -sdk iphonesimulator --show-sdk-path)
IPHONE_SIMULATOR_CROSS_TOP=${IPHONE_SIMULATOR_SDK_PATH//\/SDKs*/}
IPHONE_SIMULATOR_CROSS_SDK=${IPHONE_SIMULATOR_SDK_PATH##*/}

ARCH_LIST=("armv7" "arm64" "i386" "x86_64")
ARCH_COUNT=${#ARCH_LIST[@]}
CROSS_TOP_LIST=(${IPHONE_OS_CROSS_TOP} ${IPHONE_OS_CROSS_TOP} ${IPHONE_SIMULATOR_CROSS_TOP} ${IPHONE_SIMULATOR_CROSS_TOP})
CROSS_SDK_LIST=(${IPHONE_OS_CROSS_SDK} ${IPHONE_OS_CROSS_SDK} ${IPHONE_SIMULATOR_CROSS_SDK} ${IPHONE_SIMULATOR_CROSS_SDK})

config_make()
{
ARCH=$1;
export CROSS_TOP=$2
export CROSS_SDK=$3
export CC=$(xcrun --find gcc)

if [ "$ARCH" = "i386" -o "$ARCH" = "x86_64" ]  
then  
	SDK=$IPHONE_SIMULATOR_SDK_PATH
else  
	SDK=$IPHONE_OS_SDK_PATH
fi 

export CFLAGS="-arch ${ARCH} -isysroot ${SDK} -miphoneos-version-min=7.0 -fembed-bitcode"
#export CC="${CLANG} -arch ${ARCH} -miphoneos-version-min=6.0"

make clean &> ${OPENSSL_BUILD_LOG_DIR}/make_clean.log

if [ ! -d "${OPENSSL_BUILD_DIR}/${ARCH}/lib" ]; then
    mkdir -p "${OPENSSL_BUILD_DIR}/${ARCH}/lib"
fi

echo "configure for ${ARCH}..."
./Configure iphoneos-cross --prefix=${OPENSSL_BUILD_DIR}/${ARCH} no-asm no-shared &> ${OPENSSL_BUILD_LOG_DIR}/${ARCH}-conf.log

echo "build for ${ARCH}..."
make &> ${OPENSSL_BUILD_LOG_DIR}/${ARCH}-make.log
make install_sw &> ${OPENSSL_BUILD_LOG_DIR}/${ARCH}-make-install.log

unset CC
unset CROSS_SDK
unset CROSS_TOP

echo "build finish!\n"
}

for ((i=0; i < ${ARCH_COUNT}; i++))
do
config_make ${ARCH_LIST[i]} ${CROSS_TOP_LIST[i]} ${CROSS_SDK_LIST[i]}
done

create_lib()
{
LIB_SRC=lib/$1
LIB_DST=${OPENSSL_UNIVERSAL_LIB_DIR}/$1
LIB_PATHS=( ${ARCH_LIST[@]/#/${OPENSSL_BUILD_DIR}/} )
LIB_PATHS=( ${LIB_PATHS[@]/%//${LIB_SRC}} )
lipo ${LIB_PATHS[@]} -create -output ${LIB_DST}
}

create_lib "libssl.a"
create_lib "libcrypto.a"

cp -R ${OPENSSL_BUILD_DIR}/armv7/include ${OPENSSL_BUILD_UNIVERSAL_DIR}

popd

echo "done"