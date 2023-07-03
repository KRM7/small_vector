find_package(Catch2 3 REQUIRED)

include(Catch)
include(CTest)
enable_testing()

add_executable(small_vector_test "${CMAKE_CURRENT_SOURCE_DIR}/small_vector.cpp")
target_link_libraries(small_vector_test PRIVATE Catch2::Catch2WithMain small_vector)

catch_discover_tests(small_vector_test)
