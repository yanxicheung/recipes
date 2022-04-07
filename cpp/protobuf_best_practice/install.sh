#!/bin/bash

gtest_include_url="3th/include/gtest"
gtest_lib_name="3th/lib/libgtest.a"
gtest_cmake_file="install/CMakeLists.txt"
config_file="config.cmake"

gtest_compile_type=`sed -n 2p ${config_file} | cut -d "\"" -f2`

if [ ! -d "${gtest_lib_name}" ]; then
    echo "gtest will be installed, pls wait few minites"

    if [ -d "install"  ]; then
        rm -rf install
    fi

    mkdir install
    cd install
    git clone git@github.com:yanxicheung/gtest.git
else
    echo "gtest have been installed!!"
    exit 0
fi

echo "CMAKE_MINIMUM_REQUIRED(VERSION 2.8)" >> CMakeLists.txt

if [ "${gtest_compile_type}" == "64Bits" ]; then
    echo "install compile for 64Bits"
    echo "set (CMAKE_CXX_FLAGS \"\${CMAKE_CXX_FLAGS} -m64\")" >> CMakeLists.txt 
else
    echo "install compile for 32Bits"
    echo "set (CMAKE_CXX_FLAGS \"\${CMAKE_CXX_FLAGS} -m32\")" >> CMakeLists.txt
fi

echo "ADD_SUBDIRECTORY(\"gtest\")" >> CMakeLists.txt

mkdir build
cd build
cmake ..
cmake --build .

cd ../..
echo "$PWD"
echo "install gtest libs"
cp install/build/gtest/libgtest.a 3th/lib/

if [ -d ${gtest_include_url} ]; then
    rm -rf ${gtest_include_url}
fi
cp -r install/gtest/include/gtest/ 3th/include/

rm -rf install

echo "install gtest framework done, thx for using quick start"
