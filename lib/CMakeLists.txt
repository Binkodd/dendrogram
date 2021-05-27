add_library(dendrogram_library dendrogram.hpp dendrogram.cpp)
set_property(TARGET dendrogram_library PROPERTY CXX_STANDARD 17)

find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})

target_link_libraries(dendrogram_library ${OpenCV_LIBS})