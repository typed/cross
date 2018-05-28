# 最低版本要求，必须
cmake_minimum_required (VERSION 3.0)

# 工程名
project(cross)

include_directories(${PROJECT_SOURCE_DIR}/)

# 添加链接库查找目录，库查找序按照参数顺序从向右查
link_directories(${PROJECT_BINARY_DIR}/libs)

# 设置生成的库文件目录，这里我们设置成和库目录相同
set(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/libs)

# 设置可执行文件目录，同样设成和库目录相同
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)

add_subdirectory(AES)
add_subdirectory(Sqlite)
add_subdirectory(QuickLZ)
add_subdirectory(Pcre)
add_subdirectory(TinyXml)
add_subdirectory(Lua)
add_subdirectory(Db)
add_subdirectory(Common)
add_subdirectory(Math)
add_subdirectory(Raknet)
add_subdirectory(Net)

add_subdirectory(TestAES)
add_subdirectory(TestCommon)
add_subdirectory(TestLuaBridge)
