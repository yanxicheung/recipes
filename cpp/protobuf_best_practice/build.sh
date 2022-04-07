#!/bin/bash

working_path=$PWD
config_file=config.cmake

if [ ! -d "3th/include/gtest" ]; then
   echo "pls config your project by running config.sh"
   echo "then install xUnit by running install.sh"
   exit 0
fi

echo "try build ${working_path}"
echo "*******************************************************************************"
echo "start generate cmake project..."

if [ ! -d "build" ]; then
    mkdir -p build
fi

if [ ! -f "${config_file}" ]; then
    chmod +x config.sh
    ./config.sh
fi

project=`sed -n 1p ${config_file} | cut -d "\"" -f2`
cd build
cmake -DCMAKE_BUILD_TYPE=Debug .. 

echo "*******************************************************************************"
echo "start build cmake project..."

cmake --build .

if [ $? -ne 0 ]; then
    echo "build ${working_path} fail"
    exit 1
fi

echo "*******************************************************************************"
echo "start run tests..."

./${project} --gtest_color=yes $1 $2

if [ $? -ne 0 ]; then
    echo "build ${working_path} fail"
    exit 1
fi

cd ..

echo "*******************************************************************************"
echo "build ${working_path} succ"

