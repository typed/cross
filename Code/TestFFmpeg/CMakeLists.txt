cmake_minimum_required(VERSION 3.0)

#工程名
project(TestFFmpeg)

#源文件
file(GLOB SOURCE "*.cpp" "*.c" "*.h")

#unicode字符集
add_definitions(-DUNICODE -D_UNICODE)

add_executable(TestFFmpeg ${SOURCE})

set_target_properties(TestFFmpeg PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}/../Resource")

target_link_libraries(
    TestFFmpeg
    libavcodec
    libavdevice
    libavfilter
    libavformat
    libavutil
    libswresample
    libswscale
    SDL2
    SDL2main
)