PROJECT(dendrogram_library)
add_library(dendrogram_library SHARED dendrogram.cpp)
set_property(TARGET dendrogram_library PROPERTY CXX_STANDARD 17)

find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})

target_link_libraries(dendrogram_library ${OpenCV_LIBS})

set_target_properties(${PROJECT_NAME}
        PROPERTIES
        PUBLIC_HEADER dendrogram.hpp
        OUTPUT_NAME "dendrogram"
        CXX_STANDARD 11
        )

target_include_directories(${PROJECT_NAME} PRIVATE .)

install(TARGETS ${PROJECT_NAME}
        CONFIGURATIONS Debug
        LIBRARY
        DESTINATION dbg/lib
        PUBLIC_HEADER
        DESTINATION dbg/include)
