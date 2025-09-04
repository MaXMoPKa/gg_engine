cmake_minimum_required(VERSION 3.31.5)

project(gg_engine)

add_subdirectory(core)
#add_subdirectory(render)

target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
