# This is converter generated file, and shall not be touched by user
#
# It is always re-generated if converter script is called multiple times
# Use CMakeLists.txt to apply user changes
cmake_minimum_required(VERSION 3.22)

# Sources
set(sources_SRCS ${sources_SRCS}
    ${CMAKE_CURRENT_SOURCE_DIR}/main.c
    ${CMAKE_CURRENT_SOURCE_DIR}/filter_algo/filter_algo.c
    ${CMAKE_CURRENT_SOURCE_DIR}/func/fifo.c

)

# Include directories
set(include_c_DIRS ${include_c_DIRS}
    ${CMAKE_CURRENT_SOURCE_DIR}/
    ${CMAKE_CURRENT_SOURCE_DIR}/filter_algo/
    ${CMAKE_CURRENT_SOURCE_DIR}/func/
)
set(include_cxx_DIRS ${include_cxx_DIRS}{{sr:include_cxx_DIRS}})
set(include_asm_DIRS ${include_asm_DIRS}{{sr:include_asm_DIRS}})

# Symbols definition
set(symbols_c_SYMB ${symbols_c_SYMB})
set(symbols_cxx_SYMB ${symbols_cxx_SYMB}{{sr:symbols_cxx_SYMB}})
set(symbols_asm_SYMB ${symbols_asm_SYMB}{{sr:symbols_asm_SYMB}})

# Compiler options
set(compiler_OPTS ${compiler_OPTS})

