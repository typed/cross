cmake_minimum_required(VERSION 3.0)

#工程名
project(libPcre)

#源文件
file(GLOB SOURCE "*.cpp" "*.c" "*.h")

#unicode字符集
add_definitions(-DUNICODE -D_UNICODE)

add_library(libPcre STATIC ${SOURCE})