find_package(benchmark REQUIRED)

add_executable(small_vector_benchmark "${CMAKE_CURRENT_SOURCE_DIR}/small_vector.cpp")
target_link_libraries(small_vector_benchmark PRIVATE small_vector benchmark::benchmark_main)
