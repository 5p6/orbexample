cmake_minimum_required(VERSION 3.0)
project(zed)
find_package(OpenCV REQUIRED)
aux_source_directory(. CODE)
add_executable(main ${CODE})
option(SHOWPICURE "show picure" ON)
if(SHOWPICURE)
    message("this code will show the save picure")
    add_definitions(-DSHOWPICURE)
endif()
include_directories(${OpenCV_INCLUDE_DIRS})
target_link_libraries(main ${OpenCV_LIBS})


