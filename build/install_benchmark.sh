#!/bin/bash

echo -e "Installing google-benchmark...\n"

cmake --version

BUILD_DIR=$(dirname $(realpath "$0"))/../build
echo -e "\nThe build directory is ${BUILD_DIR}.\n"

git clone -b v1.8.1 https://github.com/google/benchmark.git $BUILD_DIR/benchmark
BENCHMARK_BUILD_DIR=$BUILD_DIR/benchmark/build
echo -e "\nThe Catch build directory is ${BENCHMARK_BUILD_DIR}.\n"

echo -e "Installing Debug configuration.\n"
cmake -S $BENCHMARK_BUILD_DIR/.. -B $BENCHMARK_BUILD_DIR -DCMAKE_BUILD_TYPE=Debug -DBENCHMARK_ENABLE_GTEST_TESTS=OFF "$@"
cmake --build $BENCHMARK_BUILD_DIR --config Debug --parallel
cmake --install $BENCHMARK_BUILD_DIR --config Debug

echo -e "Installing RelWithDebInfo configuration.\n"
cmake -S $BENCHMARK_BUILD_DIR/.. -B $BENCHMARK_BUILD_DIR -DCMAKE_BUILD_TYPE=RelWithDebInfo -DBENCHMARK_ENABLE_GTEST_TESTS=OFF "$@"
cmake --build $BENCHMARK_BUILD_DIR --config RelWithDebInfo --parallel
cmake --install $BENCHMARK_BUILD_DIR --config RelWithDebInfo

echo -e "Installing Release configuration.\n"
cmake -S $BENCHMARK_BUILD_DIR/.. -B $BENCHMARK_BUILD_DIR -DCMAKE_BUILD_TYPE=Release -DBENCHMARK_ENABLE_GTEST_TESTS=OFF "$@"
cmake --build $BENCHMARK_BUILD_DIR --config Release --parallel
cmake --install $BENCHMARK_BUILD_DIR --config Release