#语法规则 指令(参数1 参数2…) 参数之间采用空格或分号进行间隔
#设置cmake最小版本
cmake_minimum_required(VERSION 3.5) 

#打印cmake版本
message(STATUS "CMake version: ${CMAKE_VERSION}")

#指定工程采用的语言，选项分别表示：C++, C, java， 如不指定默认支持所有语言。
project(PublicMain LANGUAGES CXX C)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

#扫描当前C文件，添加到SOURCE_NAME变量内
aux_source_directory(. SOURCE_NAME)
message(STATUS "${SOURCE_NAME}")

#指定输出路径
message(STATUS "${PROJECT_SOURCE_DIR}")
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/build)#
#设置库文件的输出目录
set(LIBRARY_OUTPUT_PATH ${PROJECT_ROOT_DIR}/lib)
message(STATUS "${EXECUTABLE_OUTPUT_PATH}")

add_executable(${PROJECT_NAME}  ${SOURCE_NAME})