add_library(dendogram_library dendogram.hpp dendogram.cpp)
set_property(TARGET dendogram_library PROPERTY CXX_STANDARD 17)

find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})

target_link_libraries(dendogram_library ${OpenCV_LIBS})