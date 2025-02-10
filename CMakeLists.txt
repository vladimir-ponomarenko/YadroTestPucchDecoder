cmake_minimum_required(VERSION 3.15)
project(pucch_decoder VERSION 1.0
    DESCRIPTION "PUCCH Format 2 Decoder Implementation"
    LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_compile_options(
    -Wall
    -Wextra
    -Wpedantic
    -Wconversion
    -Wshadow
    -Werror
    -Wuninitialized
    -Wunused
)

# OpenMP
find_package(OpenMP)


add_library(pucch_core
    src/core/channel.cpp
    src/core/decoder.cpp
    src/core/encoder.cpp
    src/core/matrix.cpp
    src/core/orchestrator.cpp
    src/utils/statistics.cpp
)

target_include_directories(pucch_core
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src
)

if(OpenMP_CXX_FOUND)
    target_link_libraries(pucch_core PRIVATE OpenMP::OpenMP_CXX)
endif()

add_executable(pucch_decoder src/main.cpp)
target_link_libraries(pucch_decoder PRIVATE pucch_core)

if(OpenMP_CXX_FOUND)
    target_link_libraries(pucch_decoder PRIVATE OpenMP::OpenMP_CXX)
endif()

install(TARGETS pucch_decoder pucch_core
    RUNTIME DESTINATION bin
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
)