cmake_minimum_required(VERSION 3.0)

#工程名
project(TestLuaBridge)

#源文件
file(GLOB SOURCE "./*.cpp" "./*.h")

#unicode字符集
add_definitions(-DUNICODE -D_UNICODE)

add_executable(TestLuaBridge ${SOURCE})

target_link_libraries(TestLuaBridge
    libLua
)