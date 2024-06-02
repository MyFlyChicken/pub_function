# This is converter generated file, and shall not be touched by user
#
# It is always re-generated if converter script is called multiple times
# Use CMakeLists.txt to apply user changes
cmake_minimum_required(VERSION 3.22)

# Sources
set(sources_SRCS ${sources_SRCS}
    ${CMAKE_CURRENT_SOURCE_DIR}/main.c
    ${CMAKE_CURRENT_SOURCE_DIR}/filter_algo/filter_algo.c
    ${CMAKE_CURRENT_SOURCE_DIR}/mr_mem/mr_mem.c
    ${CMAKE_CURRENT_SOURCE_DIR}/device_driver/ee_device.c
    ${CMAKE_CURRENT_SOURCE_DIR}/func/fifo.c
    ${CMAKE_CURRENT_SOURCE_DIR}/func/mulit_blink.c
    ${CMAKE_CURRENT_SOURCE_DIR}/test/test.c
    ${CMAKE_CURRENT_SOURCE_DIR}/test/test_eb.c
    ${CMAKE_CURRENT_SOURCE_DIR}/test/test_filter.c
    ${CMAKE_CURRENT_SOURCE_DIR}/test/test_c_language.c
    ${CMAKE_CURRENT_SOURCE_DIR}/test/test_blink.c
    ${CMAKE_CURRENT_SOURCE_DIR}/test/test_mr_mem.c
    ${CMAKE_CURRENT_SOURCE_DIR}/test/test_ee_device.c
    ${CMAKE_CURRENT_SOURCE_DIR}/UnityTesting/src/unity.c
    ${CMAKE_CURRENT_SOURCE_DIR}/flash/eb.c
)

message(${CMAKE_CURRENT_SOURCE_DIR})
# Include directories
set(include_c_DIRS ${include_c_DIRS}
    ${CMAKE_CURRENT_SOURCE_DIR}/filter_algo
    ${CMAKE_CURRENT_SOURCE_DIR}/func
    ${CMAKE_CURRENT_SOURCE_DIR}/mr_mem
    ${CMAKE_CURRENT_SOURCE_DIR}/device_driver
    ${CMAKE_CURRENT_SOURCE_DIR}/test
    ${CMAKE_CURRENT_SOURCE_DIR}/UnityTesting/src
    ${CMAKE_CURRENT_SOURCE_DIR}/UnityTesting/examples
    ${CMAKE_CURRENT_SOURCE_DIR}/flash/inc
    ${CMAKE_CURRENT_SOURCE_DIR}/flash
    ${CMAKE_CURRENT_SOURCE_DIR}
)
message(${include_c_DIRS})
set(include_cxx_DIRS ${include_cxx_DIRS}{{sr:include_cxx_DIRS}})
set(include_asm_DIRS ${include_asm_DIRS}{{sr:include_asm_DIRS}})

# Symbols definition
set(symbols_c_SYMB ${symbols_c_SYMB})
set(symbols_cxx_SYMB ${symbols_cxx_SYMB}{{sr:symbols_cxx_SYMB}})
set(symbols_asm_SYMB ${symbols_asm_SYMB}{{sr:symbols_asm_SYMB}})

# Compiler options
set(compiler_OPTS ${compiler_OPTS})

