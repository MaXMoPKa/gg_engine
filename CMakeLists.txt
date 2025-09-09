cmake_minimum_required(VERSION 3.31.5)

project(gg_engine)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

#add_subdirectory(programs/gg_studio)
add_subdirectory(modules)

add_subdirectory(programs/gg_studio)

#set(TESTS OFF CACHE BOOL "" ${REWRITE_FORCE})

#if(${TESTS})
#    enable_testing()
#    add_subdirectory(tests)
#endif()
