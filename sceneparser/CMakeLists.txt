cmake_minimum_required(VERSION 3.14) 

project(sceneparser LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS NO)

#-------------
# Locate files
#-------------

set(SOURCES
    src/SceneParser.cpp
    )

#---------------
# build commands
#---------------

add_library(${PROJECT_NAME} STATIC ${SOURCES})

target_include_directories(${PROJECT_NAME} PUBLIC ${PROJECT_SOURCE_DIR}/include ../objparser/include)
