cmake_minimum_required(VERSION 3.14)

project(Renderer LANGUAGES CXX)

# for qt library
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

#-------------
# Locate files
#-------------

set(SOURCES
    src/Renderer.cpp
    src/MeshLoader.cpp
)

#---------------
# build commands
#---------------

add_executable(${PROJECT_NAME}
     ${SOURCES} 
)

target_include_directories(${PROJECT_NAME} PRIVATE ${PROJECT_SOURCE_DIR}/include)

find_package(Qt5 COMPONENTS Core Gui Quick REQUIRED)

target_compile_definitions(${PROJECT_NAME}
  PRIVATE $<$<OR:$<CONFIG:Debug>,$<CONFIG:RelWithDebInfo>>:QT_QML_DEBUG>)
target_link_libraries(${PROJECT_NAME}
  PRIVATE objparser
  Qt${QT_VERSION_MAJOR}::Core Qt${QT_VERSION_MAJOR}::Gui Qt${QT_VERSION_MAJOR}::Quick)

add_subdirectory(objparser)

set(${LIBRARY_NAME} PROPERTY
    CMAKE_CXX_STANDARD 11
    CMAKE_CXX_STANDARD_REQUIRED ON
    CXX_EXTENSIONS NO
    )

