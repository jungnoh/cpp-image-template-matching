cmake_minimum_required(VERSION 3.23)
project(imgTemplateMatch)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_FLAGS_RELEASE "-O3")

include_directories("lib")
add_executable(imgTemplateMatch src/main.cpp src/matcher.cpp)
