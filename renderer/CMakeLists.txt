cmake_minimum_required(VERSION 3.14)

project(renderer LANGUAGES CXX)

# for qt library
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

#-------------
# Locate files
#-------------

set(SOURCES
    src/MeshLoader.cpp
    src/FloatingHorizon.cpp
    src/VecUtils.cpp
)

#add headers with moc
set(HEADERS
    include/FloatingHorizon.h
    include/MeshLoader.h
    )


#---------------
# build commands
#---------------

add_library(${PROJECT_NAME}
     ${SOURCES} ${HEADERS}
)

target_include_directories(${PROJECT_NAME} PUBLIC ${PROJECT_SOURCE_DIR}/include ../objparser/include )

find_package(Qt5 COMPONENTS Core Gui Quick REQUIRED)

target_compile_definitions(${PROJECT_NAME}
  PRIVATE $<$<OR:$<CONFIG:Debug>,$<CONFIG:RelWithDebInfo>>:QT_QML_DEBUG>)
target_link_libraries(${PROJECT_NAME}
  PRIVATE objparser
  Qt${QT_VERSION_MAJOR}::Core Qt${QT_VERSION_MAJOR}::Gui Qt${QT_VERSION_MAJOR}::Quick)

set(${LIBRARY_NAME} PROPERTY
    CMAKE_CXX_STANDARD 17
    CMAKE_CXX_STANDARD_REQUIRED ON
    CXX_EXTENSIONS NO
    )

