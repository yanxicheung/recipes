project(cpa_demo)

cmake_minimum_required(VERSION 2.8)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++1y")

include_directories( 
"${CMAKE_CURRENT_SOURCE_DIR}/include/infra/base"
"${CMAKE_CURRENT_SOURCE_DIR}/include/infra/log"
"${CMAKE_CURRENT_SOURCE_DIR}/include/pb"
)

file(GLOB_RECURSE all_files
    source/*.cpp
    source/*.cxx
    source/*.cc
    source/*.c
    source/*.C
    source/infra/base/*.cpp
    source/infra/base/*.cxx
    source/infra/base/*.cc
    source/infra/base/*.c
    source/infra/base/*.C
    source/pb/*.cc)

add_executable(cpa_demo ${all_files})
target_link_libraries(cpa_demo pthread protobuf stdc++)