cmake_minimum_required(VERSION 3.10)
project(pp)
set(CMAKE_C_STANDARD 99)#C99

include_directories(${PROJECT_SOURCE_DIR}/inc)

file(GLOB_RECURSE INCLUDES inc/*.h)
file(GLOB_RECURSE SOURCES  src/*c)

find_package(Threads REQUIRED pthread)

add_library(pp STATIC ${SOURCES} ${INCLUDE})
