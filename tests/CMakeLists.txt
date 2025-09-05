cmake_minimum_required(VERSION 3.31.5)

project(gg_engine_tests)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

if(WIN32)
    include(FetchContent)
    FetchContent_Declare(
        googletest
        GIT_REPOSITORY https://github.com/google/googletest.git
        GIT_TAG        b796f7d44681514f58a683a3a71ff17c94edb0c1 # v1.13.0
    )
    set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
    FetchContent_MakeAvailable(googletest)
    add_test(NAME ${PROJECT_NAME} COMMAND ${PROJECT_NAME})
endif(WIN32)

if(WIN32)
    set(LIBS gg_engine gtest_main gtest gmock gmock_main)
else()
    set(LIBS )
endif(WIN32)

set(TYPES core)

set(core vector)

function(create_project_from_sources type test)
    file(GLOB SOURCE main.cpp
                     "${type}/${test}/*.hpp"
                     "${type}/${test}/*.cpp")

    if(test STREQUAL "")
        set(replaced "")
        string(REPLACE "/" "_" replaced ${type})
        set(NAME ${replaced})
    else()
        set(NAME "${type}__${test}")
    endif()

    add_executable(${NAME} ${SOURCE})
    target_link_libraries(${NAME} PUBLIC ${LIBS})

    if(MSVC)
        target_compile_options(${NAME} PRIVATE /std:c++23 /experimental:module)
    elseif(CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
        target_compile_options(${NAME} PRIVATE -std=c++23
                                               -fmodules)
                                               #-fbuiltin-module-map
                                               #-fimplicit-module-maps)
    elseif(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
        target_compile_options(${NAME} PRIVATE -std=c++23 -fmodules-ts)
endif()


    if(WIN32)
        set_target_properties(${NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin/tests/${type}")
        set_target_properties(${NAME} PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_BINARY_DIR}/bin/tests/${type}/Debug")
    endif(WIN32)

endfunction()

foreach(TYPE ${TYPES})
    foreach(TEST ${${TYPE}})
		create_project_from_sources(${TYPE} ${TEST})
    endforeach(TEST)
endforeach(TYPE)

include_directories(${CMAKE_SOURCE_DIR}/modules)
