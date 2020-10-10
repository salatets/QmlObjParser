cmake_minimum_required(VERSION 3.14)

project(ParseTest LANGUAGES CXX)

add_library(doctest INTERFACE)
target_include_directories(doctest INTERFACE ${CMAKE_SOURCE_DIR}/extern/doctest/doctest)

#-------------
# Locate files
#-------------

set(TESTFILES
    main.cpp
)

set(TEST_DATA
    test_data/dahl.obj
    test_data/dahl.mtl
    test_data/monkey.obj
    test_data/monkey.mtl
    test_data/cube.obj
    test_data/cube.mtl
    test_data/cube.bmp
    test_data/bgr.bmp
    test_data/bgra.bmp
)

set(TEST_MAIN unit_tests)
set(TEST_RUNNER_PARAMS "")

#---------------
# build commands
#---------------

add_executable(${TEST_MAIN} ${TESTFILES})
target_link_libraries(${TEST_MAIN} PRIVATE ${LIBS_FOR_TEST} doctest)
set_target_properties(${TEST_MAIN} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR})

set_target_properties(${TEST_MAIN} PROPERTIES
    CXX_STANDARD 14
    CXX_STANDARD_REQUIRED ON
    CXX_EXTENSIONS NO
)

add_test(
    # Use some per-module/project prefix so that it is easier to run only tests for this module
    NAME ${LIBRARY_NAME}.${TEST_MAIN}
    COMMAND ${TEST_MAIN} ${TEST_RUNNER_PARAMS})

find_package(Qt5 COMPONENTS Core Gui REQUIRED)

target_link_libraries(${TEST_MAIN}
  PRIVATE Qt${QT_VERSION_MAJOR}::Core Qt${QT_VERSION_MAJOR}::Gui )


foreach(data ${TEST_DATA})
    file(COPY ${data} DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
endforeach()
