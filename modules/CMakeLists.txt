cmake_minimum_required(VERSION 3.31.5)

project(gg_engine)

add_subdirectory(core)
add_subdirectory(render)

add_library(${PROJECT_NAME} INTERFACE)

target_link_libraries(${PROJECT_NAME} INTERFACE gg::core gg::render)

target_include_directories(${PROJECT_NAME} INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})
