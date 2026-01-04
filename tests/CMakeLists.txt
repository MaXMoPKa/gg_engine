cmake_minimum_required(VERSION 3.31.5)

project(gg_engine_tests)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

if(WIN32)
    include(FetchContent)
    FetchContent_Declare(
        googletest
        GIT_REPOSITORY https://github.com/google/googletest.git
        GIT_TAG        v1.17.0
    )
    set(BUILD_GMOCK OFF CACHE BOOL "Builds the googlemock library" FORCE)
    set(INSTALL_GTEST OFF CACHE BOOL "Enable installation of googletest. (Projects embedding googletest may want to turn this OFF.)" FORCE)
    set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
    FetchContent_MakeAvailable(googletest)
endif(WIN32)

set(GTEST_DLLS
    $<TARGET_FILE_DIR:gtest>/$<TARGET_FILE_NAME:gtest>
    $<TARGET_FILE_DIR:gtest_main>/$<TARGET_FILE_NAME:gtest_main>
)

set(LOG4CPLUS_DLL ${CMAKE_BINARY_DIR}/modules/third_party/log4cplus/bin/log4cplusD.dll)

if(WIN32)
    set(LIBS gg_engine gtest_main gtest)
else()
    set(LIBS )
endif(WIN32)

set(TYPES log)
set(log logger)

function(create_project_from_sources type test)
    file(GLOB MODULE_SOURCE "${type}/${test}/*.cppm")
    if(NOT MODULE_SOURCE)
      message(WARNING "No module source file fount for ${type}/${test}/*.cppm")
      return()
    endif()

    set(NAME "${type}_${test}_test")

    add_executable(${NAME} main.cpp)

    target_sources(${NAME} PUBLIC FILE_SET cxx_modules TYPE CXX_MODULES FILES ${MODULE_SOURCE})

    set_target_properties(${NAME} PROPERTIES CXX_MODULE_LINKED TRUE)
    
    source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${MODULE_SOURCE})
    
    target_link_libraries(${NAME} PUBLIC ${LIBS})

    if(WIN32)
        set_target_properties(${NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin/tests/${type}")
        set_target_properties(${NAME} PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_BINARY_DIR}/bin/tests/${type}/Debug")
    endif(WIN32)

    add_custom_command(TARGET ${NAME} POST_BUILD
      COMMAND ${CMAKE_COMMAND} -E copy_if_different
      ${GTEST_DLLS}
      ${LOG4CPLUS_DLL}
      $<TARGET_FILE_DIR:${NAME}>
      COMMENT "Copying Google Test DLLs to test executable directory")

endfunction()

foreach(TYPE ${TYPES})
    foreach(TEST ${${TYPE}})
		create_project_from_sources(${TYPE} ${TEST})
    endforeach(TEST)
endforeach(TYPE)
