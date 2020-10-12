cmake_minimum_required(VERSION 3.14)

project(QmlObjParser LANGUAGES CXX)

# for qt library
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

#-------------
# Locate files
#-------------

set(SOURCES
    main.cpp
    src/glscene.cpp
    qml.qrc
)
#add headers with moc
set(HEADERS
    include/glscene.h
    )

set(SHADERS "${PROJECT_SOURCE_DIR}/include/shaders.h")

#---------------
# build commands
#---------------

add_executable(${PROJECT_NAME}
     ${SOURCES} ${HEADERS}
)

target_include_directories(${PROJECT_NAME} PRIVATE ${PROJECT_SOURCE_DIR}/include)

find_package(Qt5 COMPONENTS Core Gui Quick REQUIRED)

target_compile_definitions(${PROJECT_NAME}
  PRIVATE $<$<OR:$<CONFIG:Debug>,$<CONFIG:RelWithDebInfo>>:QT_QML_DEBUG>)
target_link_libraries(${PROJECT_NAME}
  PRIVATE renderer objparser
  Qt${QT_VERSION_MAJOR}::Core Qt${QT_VERSION_MAJOR}::Gui Qt${QT_VERSION_MAJOR}::Quick)

add_subdirectory(renderer)
add_subdirectory(objparser)

set(${LIBRARY_NAME} PROPERTY
    CMAKE_CXX_STANDARD 11
    CMAKE_CXX_STANDARD_REQUIRED ON
    CXX_EXTENSIONS NO
    )

#-----------------------------------
# generate shaders header from files
#-----------------------------------
add_custom_command(
    OUTPUT "${SHADERS}"
    COMMAND python utils/fileToString.py -i shaders/fragment.glsl shaders/vertex.glsl shaders/hor_vertex.glsl shaders/hor_fragment.glsl -o "${SHADERS}"
    DEPENDS shaders/fragment.glsl shaders/vertex.glsl shaders/hor_vertex.glsl shaders/hor_fragment.glsl
    WORKING_DIRECTORY "${PROJECT_SOURCE_DIR}"
)
set_property(SOURCE src/glscene.cpp APPEND PROPERTY OBJECT_DEPENDS "${SHADERS}")





