# 最低版本要求，必须
cmake_minimum_required (VERSION 3.0)

if(WIN32) # Win32
  set(PLATFORM_FOLDER win32)
elseif(APPLE)# osx or ios
  set(PLATFORM_FOLDER mac)
else() # Assume Linux
  set(PLATFORM_FOLDER linux)
endif()

# 工程名
project(cross)

include_directories(
    ${PROJECT_SOURCE_DIR}/
    ${CMAKE_CURRENT_SOURCE_DIR}/FFmpeg/include
    ${CMAKE_CURRENT_SOURCE_DIR}/SDL2/include
)

# 添加链接库查找目录，库查找序按照参数顺序从向右查
link_directories(
    ${PROJECT_BINARY_DIR}/libs
    ${CMAKE_CURRENT_SOURCE_DIR}/FFmpeg/lib/${PLATFORM_FOLDER}
    ${CMAKE_CURRENT_SOURCE_DIR}/SDL2/lib/${PLATFORM_FOLDER}
)

# 设置生成的库文件目录，这里我们设置成和库目录相同
set(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/libs)

# 设置可执行文件目录，同样设成和库目录相同
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)

# lua版本目录
#set(LUAVER Lua515)
set(LUAVER Lua535)

add_subdirectory(AES)
add_subdirectory(Sqlite)
add_subdirectory(QuickLZ)
add_subdirectory(Pcre)
add_subdirectory(TinyXml)
add_subdirectory(Lua)
add_subdirectory(ToLuapp)
add_subdirectory(Db)
add_subdirectory(Common)
add_subdirectory(Math)
add_subdirectory(Raknet)
add_subdirectory(Net)
add_subdirectory(Exception)

add_subdirectory(TestAES)
add_subdirectory(TestTinyXml)
add_subdirectory(TestCommon)
add_subdirectory(TestLua)
add_subdirectory(TestFFmpeg)
