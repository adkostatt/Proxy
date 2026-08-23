cmake_minimum_required(VERSION 3.20)

project("Proxy")

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

find_package(Http REQUIRED)
find_package(SmallBeer REQUIRED)

add_subdirectory(src)
