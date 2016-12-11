cmake_minimum_required(VERSION 2.8 FATAL_ERROR)

project(Db18b20_tests)

enable_language(C CXX)

set(GOOGLETEST_ROOT ../../External/googletest/googletest CACHE STRING "Google Test source root")

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x")

include_directories(
    ${PROJECT_SOURCE_DIR}/${GOOGLETEST_ROOT}
    ${PROJECT_SOURCE_DIR}/${GOOGLETEST_ROOT}/include
    ../
    ../../External/include
    ../../External/testing
    )

set(GOOGLETEST_SOURCES
    ${PROJECT_SOURCE_DIR}/${GOOGLETEST_ROOT}/src/gtest-all.cc
    ${PROJECT_SOURCE_DIR}/${GOOGLETEST_ROOT}/src/gtest_main.cc
    )

foreach(_source ${GOOGLETEST_SOURCES})
    set_source_files_properties(${_source} PROPERTIES GENERATED 1)
endforeach()

add_library(googletest ${GOOGLETEST_SOURCES})

add_executable(
    Ds18b20_unit_tests
    main.cc
    Ds18b20Tests.cc
    ../Ds18b20.cpp
    )

add_dependencies(Ds18b20_unit_tests googletest)

target_link_libraries(
    Ds18b20_unit_tests
    googletest
    pthread
    )

