include(GoogleTest)

add_executable(test_libseed test_libseed.cpp)

target_link_libraries(test_libseed PRIVATE libseed GTest::gtest_main)

target_compile_options(test_libseed PRIVATE -Wall -Wextra -Wpedantic)

gtest_discover_tests(test_libseed)
