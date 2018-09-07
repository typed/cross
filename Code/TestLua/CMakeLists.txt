cmake_minimum_required(VERSION 3.0)

#工程名
project(TestLua)

#源文件
file(GLOB SOURCE "*.cpp" "*.c" "*.h")

#unicode字符集
add_definitions(-DUNICODE -D_UNICODE)

include_directories(../Common)
include_directories(../Lua/${LUAVER})
include_directories(../ToLuapp)
include_directories(../LuaBridge)

add_executable(TestLua ${SOURCE})

set_target_properties(TestLua PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}/../Resource")

target_link_libraries(TestLua
    libLua
    libToLuapp
    libMath
    libPcre
    libCommon
)

