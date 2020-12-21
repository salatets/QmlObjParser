cmake_minimum_required(VERSION 3.14)

project(renderer LANGUAGES CXX)

# for qt library
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS NO)

#-------------
# Locate files
#-------------

set(SOURCES
    src/MeshLoader.cpp
    src/FloatingHorizon.cpp
    src/MeshNodeLoader.cpp
    src/MeshNodeLoaders.cpp
    src/SceneLoader.cpp
    src/vecUtils.cpp
    )

#add headers with moc
set(HEADERS
    include/FloatingHorizon.h
    include/MeshLoader.h
    include/MeshNodeLoader.h
    include/MeshNodeLoaders.h
    include/SceneLoader.h
    include/ShaderHolder.h
    )


#---------------
# build commands
#---------------

add_library(${PROJECT_NAME}
    ${SOURCES} ${HEADERS}
    )

target_include_directories(${PROJECT_NAME} PUBLIC ${PROJECT_SOURCE_DIR}/include ../objparser/include )

find_package(QT NAMES Qt6 Qt5 COMPONENTS Core Gui Quick OpenGL REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Core Gui Quick OpenGL REQUIRED)

target_compile_definitions(${PROJECT_NAME}
    PRIVATE $<$<OR:$<CONFIG:Debug>,$<CONFIG:RelWithDebInfo>>:QT_QML_DEBUG>)
target_link_libraries(${PROJECT_NAME}
    PUBLIC objparser sceneparser
    Qt${QT_VERSION_MAJOR}::Core Qt${QT_VERSION_MAJOR}::Gui Qt${QT_VERSION_MAJOR}::Quick Qt${QT_VERSION_MAJOR}::OpenGL)


