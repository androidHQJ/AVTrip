
#ffmpeg 3.4.5

cmake_minimum_required(VERSION 3.4.1)
#源码树的顶层路径。
set(JNI_LIBS_DIR ${CMAKE_SOURCE_DIR}/src/main/jniLibs)

# 查看gradle Console 调试信息
message(STATUS "========> the PROJECT_SOURCE_DIR is : ${PROJECT_SOURCE_DIR}")
message(STATUS "========> the PROJECT_BINARY_DIR is : ${PROJECT_BINARY_DIR}")
message(STATUS "========> the cmakeList.txt PATH is : ${CMAKE_CURRENT_SOURCE_DIR}")
message(STATUS "========> CMAKE_SOURCE_DIR is : ${CMAKE_SOURCE_DIR}")
message(STATUS "========> JNI_LIBS_DIR is : ${JNI_LIBS_DIR}")

# 个人库
add_library(native-lib SHARED src/main/cpp/native-lib.cpp)

# 系统库
find_library(log-lib log)
find_library(android-lib android )

# 第三方库
add_library(avcodec SHARED IMPORTED)
set_target_properties(
        avcodec
        PROPERTIES IMPORTED_LOCATION
        # ${ANDROID_ABI}是根据gradle中声明的abiFilters来指定的。
        ${JNI_LIBS_DIR}/${ANDROID_ABI}/libavcodec.so)

add_library(avdevice SHARED IMPORTED)
set_target_properties(
        avdevice
        PROPERTIES IMPORTED_LOCATION
        # ${ANDROID_ABI}是根据gradle中声明的abiFilters来指定的。
        ${JNI_LIBS_DIR}/${ANDROID_ABI}/libavdevice.so)

add_library(avfilter SHARED IMPORTED)
set_target_properties(
        avfilter
        PROPERTIES IMPORTED_LOCATION
        # ${ANDROID_ABI}是根据gradle中声明的abiFilters来指定的。
        ${JNI_LIBS_DIR}/${ANDROID_ABI}/libavfilter.so)

add_library(avformat SHARED IMPORTED)
set_target_properties(
        avformat
        PROPERTIES IMPORTED_LOCATION
        # ${ANDROID_ABI}是根据gradle中声明的abiFilters来指定的。
        ${JNI_LIBS_DIR}/${ANDROID_ABI}/libavformat.so)

add_library(avutil SHARED IMPORTED)
set_target_properties(
        avutil
        PROPERTIES IMPORTED_LOCATION
        # ${ANDROID_ABI}是根据gradle中声明的abiFilters来指定的。
        ${JNI_LIBS_DIR}/${ANDROID_ABI}/libavutil.so)

add_library(swresample SHARED IMPORTED)
set_target_properties(
        swresample
        PROPERTIES IMPORTED_LOCATION
        # ${ANDROID_ABI}是根据gradle中声明的abiFilters来指定的。
        ${JNI_LIBS_DIR}/${ANDROID_ABI}/libswresample.so)

add_library(swscale SHARED IMPORTED)
set_target_properties(
        swscale
        PROPERTIES IMPORTED_LOCATION
        # ${ANDROID_ABI}是根据gradle中声明的abiFilters来指定的。
        ${JNI_LIBS_DIR}/${ANDROID_ABI}/libswscale.so)

# 头文件路径
include_directories(${JNI_LIBS_DIR}/includes)

target_link_libraries(native-lib
        avcodec avfilter avformat avutil swresample swscale avdevice
        ${log-lib}
        ${android-lib}
)