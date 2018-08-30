
#工程名
project(libLua)

#源文件
file(GLOB SOURCE "*.cpp" "*.c" "*.h")
#MESSAGE(STATUS "this var = ${SOURCE}.")
list(REMOVE_ITEM SOURCE "${CMAKE_CURRENT_SOURCE_DIR}/lua.c" "${CMAKE_CURRENT_SOURCE_DIR}/luac.c")

#unicode字符集
add_definitions(-DUNICODE -D_UNICODE)

add_library(libLua STATIC ${SOURCE})
