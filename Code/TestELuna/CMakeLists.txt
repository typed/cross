cmake_minimum_required(VERSION 3.0)

#工程名
project(TestELuna)

#源文件
file(GLOB SOURCE "./*.cpp" "./*.h")

#unicode字符集
add_definitions(-DUNICODE -D_UNICODE)

add_executable(TestELuna ${SOURCE})

set_target_properties(TestELuna PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}/../Resource")

target_link_libraries(TestELuna
    libLua
    libELuna
)