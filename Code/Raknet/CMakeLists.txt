cmake_minimum_required(VERSION 3.0)

#工程名
project(libRaknet)

#源文件
file(GLOB SOURCE "./*.cpp" "./*.h")

#unicode字符集
add_definitions(-DUNICODE -D_UNICODE)

add_library(libRaknet STATIC ${SOURCE})