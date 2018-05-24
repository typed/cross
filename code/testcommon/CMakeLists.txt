cmake_minimum_required(VERSION 3.0)

#工程名
project(testcommon)

#源文件
set(testcommon_src
    main.cpp
)

#unicode字符集
add_definitions(-DUNICODE -D_UNICODE)

add_executable(testcommon ${testcommon_src})

target_link_libraries(testcommon
    libtinyxml
)