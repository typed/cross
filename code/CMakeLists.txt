# 最低版本要求，必须
cmake_minimum_required (VERSION 3.0)

# 工程名
project(all)

include_directories(${PROJECT_SOURCE_DIR}/)

# 添加链接库查找目录，库查找序按照参数顺序从向右查
link_directories(${PROJECT_BINARY_DIR}/libs)

# 设置生成的库文件目录，这里我们设置成和库目录相同
set(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/libs)

# 设置可执行文件目录，同样设成和库目录相同
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)

add_subdirectory(tinyxml)
add_subdirectory(testcommon)