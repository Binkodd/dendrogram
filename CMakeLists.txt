cmake_minimum_required(VERSION 3.10)
project(dendrogram)

set(CMAKE_BUILD_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/build)

include_directories(lib test doc)
add_subdirectory(lib)
add_subdirectory(test)
add_subdirectory(doc)

install(DIRECTORY lib DESTINATION lib)



