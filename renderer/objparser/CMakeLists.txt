cmake_minimum_required(VERSION 3.14)

project(objparser LANGUAGES CXX)

option(MYLIB_TESTING "enable tests" ON)

#-------------
# Locate files
#-------------

set(SOURCES
    src/MeshNode.cpp
    src/ImageLoader.cpp
    src/ObjParser.cpp
)

set(LIBS_FOR_TEST ${PROJECT_NAME})

#---------------
# build commands
#---------------

add_library(${PROJECT_NAME} STATIC ${SOURCES})

target_include_directories(${PROJECT_NAME} PUBLIC ${PROJECT_SOURCE_DIR}/include)

find_package(Qt5 COMPONENTS Core Gui REQUIRED)

target_link_libraries(${PROJECT_NAME}
  PRIVATE Qt${QT_VERSION_MAJOR}::Core Qt${QT_VERSION_MAJOR}::Gui )

set(${PROJECT_NAME} PROPERTY
    CXX_STANDARD 14
    CXX_STANDARD_REQUIRED ON
    CXX_EXTENSIONS NO
    )

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

