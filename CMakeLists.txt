cmake_minimum_required(VERSION 3.20)
project(VCS_alpha LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_executable(VCS_alpha
    src/main.cpp
    src/GirafaRosaPink.cpp
)

target_include_directories(VCS_alpha PRIVATE include)

if (MSVC)
    target_compile_options(VCS_alpha PRIVATE /W4 /permissive-)
else()
    target_compile_options(VCS_alpha PRIVATE -Wall -Wextra -Wpedantic)
endif()
