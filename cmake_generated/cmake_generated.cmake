# This is converter generated file, and shall not be touched by user
#
# It is always re-generated if converter script is called multiple times
# Use CMakeLists.txt to apply user changes
cmake_minimum_required(VERSION 3.22)

# Sources
set(sources_SRCS ${sources_SRCS}
    ${CMAKE_CURRENT_SOURCE_DIR}/main.c
    ${CMAKE_CURRENT_SOURCE_DIR}/codebrick/algorithm/filter/filter_algo.c
    ${CMAKE_CURRENT_SOURCE_DIR}/codebrick/algorithm/search/algo_search.c
    ${CMAKE_CURRENT_SOURCE_DIR}/codebrick/mr_mem/mr_mem.c
    ${CMAKE_CURRENT_SOURCE_DIR}/codebrick/device_driver/ee_device.c
    ${CMAKE_CURRENT_SOURCE_DIR}/codebrick/func/fifo.c
    ${CMAKE_CURRENT_SOURCE_DIR}/codebrick/mulit/mulit_blink.c
    ${CMAKE_CURRENT_SOURCE_DIR}/codebrick/mulit/mulit_key.c
    ${CMAKE_CURRENT_SOURCE_DIR}/codebrick/flash/eb.c
    ${CMAKE_CURRENT_SOURCE_DIR}/codebrick/async_work/async_work.c
    ${CMAKE_CURRENT_SOURCE_DIR}/codebrick/func/func_queue.c
    ${CMAKE_CURRENT_SOURCE_DIR}/codebrick/oop_c/circle.c
    ${CMAKE_CURRENT_SOURCE_DIR}/codebrick/oop_c/rect.c
    ${CMAKE_CURRENT_SOURCE_DIR}/codebrick/oop_c/shape.c
     ${CMAKE_CURRENT_SOURCE_DIR}/codebrick/pubsub/src/pubsub.c
    ${CMAKE_CURRENT_SOURCE_DIR}/codebrick/pubsub/src/psqueue_b.c
    ${CMAKE_CURRENT_SOURCE_DIR}/codebrick/pubsub/src/psqueue_ll.c
    ${CMAKE_CURRENT_SOURCE_DIR}/codebrick/pubsub/src/sync_linux.c
    ${CMAKE_CURRENT_SOURCE_DIR}/codebrick/pubsub/src/sync_freertos.c
    ${CMAKE_CURRENT_SOURCE_DIR}/os/polling/src/polling.c
    ${CMAKE_CURRENT_SOURCE_DIR}/test/test.c
    ${CMAKE_CURRENT_SOURCE_DIR}/test/test_key.c
    ${CMAKE_CURRENT_SOURCE_DIR}/test/test_eb.c
    ${CMAKE_CURRENT_SOURCE_DIR}/test/test_filter.c
    ${CMAKE_CURRENT_SOURCE_DIR}/test/test_c_language.c
    ${CMAKE_CURRENT_SOURCE_DIR}/test/test_blink.c
    ${CMAKE_CURRENT_SOURCE_DIR}/test/test_mr_mem.c
    ${CMAKE_CURRENT_SOURCE_DIR}/test/test_ee_device.c
    ${CMAKE_CURRENT_SOURCE_DIR}/test/test_async.c
    ${CMAKE_CURRENT_SOURCE_DIR}/test/test_polling.c
    ${CMAKE_CURRENT_SOURCE_DIR}/test/test_func_queue.c
    ${CMAKE_CURRENT_SOURCE_DIR}/test/test_oop_c.c
    ${CMAKE_CURRENT_SOURCE_DIR}/test/test_c11_generic.c
    ${CMAKE_CURRENT_SOURCE_DIR}/test/test_binary_search.c
    ${CMAKE_CURRENT_SOURCE_DIR}/test/test_pubsub.c
    ${CMAKE_CURRENT_SOURCE_DIR}/UnityTesting/src/unity.c
   
)

message(${CMAKE_CURRENT_SOURCE_DIR})
# Include directories
set(include_c_DIRS ${include_c_DIRS}
    ${CMAKE_CURRENT_SOURCE_DIR}/codebrick/algorithm/filter
    ${CMAKE_CURRENT_SOURCE_DIR}/codebrick/algorithm/search
    ${CMAKE_CURRENT_SOURCE_DIR}/codebrick/func
    ${CMAKE_CURRENT_SOURCE_DIR}/codebrick/mr_mem
    ${CMAKE_CURRENT_SOURCE_DIR}/codebrick/device_driver
    ${CMAKE_CURRENT_SOURCE_DIR}/codebrick/flash/inc
    ${CMAKE_CURRENT_SOURCE_DIR}/codebrick/flash
    ${CMAKE_CURRENT_SOURCE_DIR}/codebrick/async_work
    ${CMAKE_CURRENT_SOURCE_DIR}/codebrick/polling/inc
    ${CMAKE_CURRENT_SOURCE_DIR}/codebrick/func
    ${CMAKE_CURRENT_SOURCE_DIR}/codebrick/mulit
    ${CMAKE_CURRENT_SOURCE_DIR}/codebrick/oop_c    
    ${CMAKE_CURRENT_SOURCE_DIR}/test
    ${CMAKE_CURRENT_SOURCE_DIR}/UnityTesting/src
    ${CMAKE_CURRENT_SOURCE_DIR}/UnityTesting/examples
    ${CMAKE_CURRENT_SOURCE_DIR}/codebrick/pubsub/src
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

