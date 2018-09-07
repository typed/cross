
#工程名
project(libLua)

#源文件
file(GLOB SOURCE "${LUAVER}/*.cpp" "${LUAVER}/*.c" "${LUAVER}/*.h")
#MESSAGE(STATUS "this var = ${SOURCE}.")
list(REMOVE_ITEM SOURCE "${CMAKE_CURRENT_SOURCE_DIR}/${LUAVER}/lua.c" "${CMAKE_CURRENT_SOURCE_DIR}/${LUAVER}/luac.c")

#unicode字符集
add_definitions(-DUNICODE -D_UNICODE)

add_library(libLua STATIC ${SOURCE})
