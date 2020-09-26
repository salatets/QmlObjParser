cmake_minimum_required(VERSION 3.14)

project(objparser LANGUAGES CXX)

option(MYLIB_TESTING "enable tests" ON)

#-------------
# Locate files
#-------------

set(SOURCES
    src/mesh.cpp
)

set(LIBRARY_NAME objparser)

#---------------
# build commands
#---------------

add_library(${LIBRARY_NAME} STATIC src/mesh.cpp)

target_include_directories(${LIBRARY_NAME} PUBLIC ${PROJECT_SOURCE_DIR}/include)

find_package(Qt5 COMPONENTS Core Gui REQUIRED)

target_link_libraries(objparser
  PRIVATE Qt5::Core Qt5::Gui )

set(${LIBRARY_NAME} PROPERTY
    CMAKE_CXX_STANDARD 11
    CMAKE_CXX_STANDARD_REQUIRED ON
    CXX_EXTENSIONS NO
    )

# for qt library
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

#------
# test fuctionality
#------

get_directory_property(IS_SUBPROJECT PARENT_DIRECTORY)

if(NOT MYLIB_TESTING)
    message(STATUS "Tests is off")
elseif(IS_SUBPROJECT)
    message(STATUS "Tests is off for submodule")
else()
    include(CTest)
    add_definitions(-DENABLE_DOCTEST_IN_LIBRARY)
    add_subdirectory(test)
endif()

