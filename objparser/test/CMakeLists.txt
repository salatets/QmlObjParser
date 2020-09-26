cmake_minimum_required(VERSION 3.5)

project(ParseTest LANGUAGES CXX)

add_definitions(-DGTEST_LANGUAGE_CXX11)

find_package(Threads REQUIRED)

add_subdirectory("${PROJECT_SOURCE_DIR}/extern/googletest" "extern/googletest")

