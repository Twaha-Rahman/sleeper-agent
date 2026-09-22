cmake_minimum_required(VERSION 3.20)

project(sleeper-agent
        VERSION 0.1.0
        DESCRIPTION "Daemon executable with a dummy libseed library."
        LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

find_package(PkgConfig REQUIRED)
pkg_check_modules(SYSTEMD REQUIRED IMPORTED_TARGET libsystemd)

include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/v1.15.2.zip
)
set(BUILD_GMOCK OFF CACHE BOOL "" FORCE)
set(INSTALL_GTEST OFF CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

enable_testing()

add_library(libseed SHARED libseed/libseed.cpp)

add_executable(${PROJECT_NAME} main.cpp)

target_link_libraries(${PROJECT_NAME} PRIVATE libseed PkgConfig::SYSTEMD)

target_include_directories(libseed PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/libseed)

target_compile_options(libseed PRIVATE
                        -Wall -Wextra -Wpedantic)

target_compile_options(${PROJECT_NAME} PRIVATE
                        -Wall -Wextra -Wpedantic)

target_compile_options(libseed PUBLIC
  $<$<CONFIG:Debug>:-O0 -g>
  $<$<CONFIG:Release>:-O3>
)

include(ClangTidy)
AddClangTidy(libseed)
AddClangTidy(${PROJECT_NAME})

include(Valgrind)
AddValgrind(${PROJECT_NAME})

add_subdirectory(tests)
