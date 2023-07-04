#!/bin/sh

git clone https://github.com/google/benchmark.git
cd benchmark
git checkout tags/v1.8.1
cmake -Bbuild -S. -DBENCHMARK_ENABLE_GTEST_TESTS=OFF
cmake --build build/ --target install --config Debug
cmake --build build/ --target install --config Release
cmake --build build/ --target install --config RelWithDebInfo
cd ..
