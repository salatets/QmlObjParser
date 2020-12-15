cmake_minimum_required(VERSION 3.14)

project(QmlObjParser LANGUAGES CXX)

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

find_package(QT NAMES Qt6 Qt5 COMPONENTS Core Gui Quick QuickControls2 REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Core Gui Quick QuickControls2 REQUIRED)

target_compile_definitions(${PROJECT_NAME}
    PRIVATE $<$<OR:$<CONFIG:Debug>,$<CONFIG:RelWithDebInfo>>:QT_QML_DEBUG>)
target_link_libraries(${PROJECT_NAME}
    PUBLIC renderer objparser sceneparser
    Qt${QT_VERSION_MAJOR}::Core Qt${QT_VERSION_MAJOR}::Gui Qt${QT_VERSION_MAJOR}::Quick Qt${QT_VERSION_MAJOR}::QuickControls2)

add_definitions( -DQT_VERSION_MAJOR=${QT_VERSION_MAJOR})

add_subdirectory(renderer)
add_subdirectory(objparser)
add_subdirectory(sceneparser)

#-----------------------------------
# generate shaders header from files
#-----------------------------------

set(SHADERS_LIST
    shaders/VNT_fragment.glsl shaders/VNT_vertex.glsl
    shaders/VN_fragment.glsl shaders/VN_vertex.glsl
    shaders/VT_fragment.glsl shaders/VT_vertex.glsl
    shaders/V_fragment.glsl shaders/V_vertex.glsl
    shaders/hor_fragment.glsl shaders/hor_vertex.glsl)

add_custom_command(
    OUTPUT "${SHADERS}"
    COMMAND python utils/fileToString.py -i ${SHADERS_LIST} -o "${SHADERS}"
    DEPENDS ${SHADERS_LIST}
    WORKING_DIRECTORY "${PROJECT_SOURCE_DIR}"
    )
set_property(SOURCE src/glscene.cpp APPEND PROPERTY OBJECT_DEPENDS "${SHADERS}")





