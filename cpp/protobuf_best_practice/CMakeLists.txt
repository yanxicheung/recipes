CMAKE_MINIMUM_REQUIRED(VERSION 2.8)

include("config.cmake")

PROJECT(${proj_name})

MACRO(sort_files source_files)
    SET(sgbd_cur_dir ${CMAKE_CURRENT_SOURCE_DIR})
    FOREACH(sgbd_file ${${source_files}})
        STRING(REGEX REPLACE ${sgbd_cur_dir}/\(.*\) \\1 sgbd_fpath ${sgbd_file})
        STRING(REGEX REPLACE "\(.*\)/.*" \\1 sgbd_group_name ${sgbd_fpath})
        STRING(COMPARE EQUAL ${sgbd_fpath} ${sgbd_group_name} sgbd_nogroup)
        IF(MSVC)
            string(REPLACE "/" "\\" sgbd_group_name ${sgbd_group_name})
        ENDIF(MSVC)
        IF(sgbd_nogroup)
            SET(sgbd_group_name "\\")
        ENDIF(sgbd_nogroup)
        SOURCE_GROUP(${sgbd_group_name} FILES ${sgbd_file})
    ENDFOREACH(sgbd_file)
ENDMACRO(sort_files)

INCLUDE_DIRECTORIES( 
"${CMAKE_CURRENT_SOURCE_DIR}/include"
"${CMAKE_CURRENT_SOURCE_DIR}/include/pb"
"${CMAKE_CURRENT_SOURCE_DIR}/test"
"${CMAKE_CURRENT_SOURCE_DIR}/3th/include"
)

IF(MSVC)
  ADD_DEFINITIONS(-D_CRT_SECURE_NO_WARNINGS )
  ADD_DEFINITIONS(-DMSVC_VMG_ENABLED)
  SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /vmg")
ENDIF(MSVC)

IF(UNIX)
  if(${c_type} STREQUAL "64Bits")
    message("-- compile this project as 64Bits")
    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -m64 ")
  ELSE()
    message("-- copmile this porject as 32Bits")
    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -m32")
  ENDIF()
ENDIF(UNIX)

FILE(GLOB_RECURSE all_files 
include/*.h
include/*.hpp
include/*.hxx
include/*.tcc
src/*.cpp
src/*.cc
src/*.cxx
src/*.c
src/*.C
test/*.h
test/*.hpp
test/*.hxx
test/*.tcc
test/*.cpp
test/*.cc
test/*.cxx
test/*.c
test/*.C)

if(APPLE)  
add_definitions(
-DGTEST_HAS_TR1_TUPLE=1
-DGTEST_USE_OWN_TR1_TUPLE=1
)
endif()

sort_files(all_files)

link_directories("${CMAKE_CURRENT_SOURCE_DIR}/3th/lib")
set(CMAKE_BUILD_TYPE ${CMAKE_BUILD_TYPE})

add_executable(${proj_name} ${all_files})

if(MSVC80)
    target_link_libraries(${proj_name} gtest-vc80)
elseif(MSVC90)
    target_link_libraries(${proj_name} gtest-vc90)
elseif(MSVC10)
    target_link_libraries(${proj_name} gtest-vc10)
elseif(APPLE)
   message("-- links this project as 64Bits")
   target_link_libraries(${proj_name} pthread stdc++)
else(UNIX)
    if(${c_type} STREQUAL "64Bits")
        message("-- links this project as 64Bits")
        target_link_libraries(${proj_name} gtest pthread protobuf stdc++)
    else()
        message("-- links this project as 32Bits")
        target_link_libraries(${proj_name} gtest pthread protobuf stdc++)
    endif()
endif()

