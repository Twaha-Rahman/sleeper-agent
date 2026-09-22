include(GoogleTest)

add_executable(test_libsleeperagent test_libsleeperagent.cpp)

target_link_libraries(test_libsleeperagent PRIVATE libsleeperagent GTest::gtest_main)

target_compile_options(test_libsleeperagent PRIVATE -Wall -Wextra -Wpedantic)

gtest_discover_tests(test_libsleeperagent)
