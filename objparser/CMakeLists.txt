cmake_minimum_required(VERSION 3.5)

project(objparser LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

option(MYLIB_TESTING "enable tests" ON)
get_directory_property(IS_SUBPROJECT PARENT_DIRECTORY)

if(NOT MYLIB_TESTING)
    message(STATUS "Tests is off")
elseif(IS_SUBPROJECT)
    message(STATUS "Tests is off for submodule")
else()
    enable_testing()
    include(GoogleTest)
    add_subdirectory(test)
endif()


find_package(Qt5 COMPONENTS Core Gui REQUIRED)

add_executable(objparser main.cpp src/mesh.cpp include/mesh.h)

target_include_directories(objparser PUBLIC include)

target_link_libraries(objparser
  PRIVATE Qt5::Core Qt5::Gui )

