
# Sets the minimum version of CMake required to build the native library.

#最低cmake版本
cmake_minimum_required(VERSION 3.4.1)

#TODO 第一步导入头文件
include_directories("inc")

#project("jni_qq")

#批量导入所有源文件
#file(GLOB allCPP *.c *.h *.cpp)

add_library( # Sets the name of the library.
        native-lib #生成libnative-lib.so
        SHARED #动态库
        native-lib.cpp
        #             ${allCPP}
        )

# TODO 导入库文件
#默认jniLibs

#cpu 架构模式
#arm64-v8a 现在
#armeabi-v7a 2015
#armeabi  2011
#x86 x86_64 模拟器，windowphone

#查看自己手机的架构模糊
#adb shell getprop ro.product.cpu.abi
#arm64-v8a 【华为p20】

#CMAKE_ANDROID_ARCH_ABI 根据系统
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -L${CMAKE_SOURCE_DIR}../jnilibs/${CMAKE_ANDROID_ARCH_ABI}")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -L${CMAKE_SOURCE_DIR}/../jniLibs/${CMAKE_ANDROID_ARCH_ABI}")
find_library( # Sets the name of the path variable.
        log-lib

        # Specifies the name of the NDK library that
        # you want CMake to locate.
        log)

# Specifies libraries CMake should link to your target library. You
# can link multiple libraries, such as libraries you define in this
# build script, prebuilt third-party libraries, or system libraries.

target_link_libraries( # Specifies the target library.
        native-lib

        # Links the target library to the log library
        # included in the NDK.
        ${log-lib}
        fmod
        fmodL
        log
        )
