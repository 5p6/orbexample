cmake_minimum_required(VERSION 3.0)
project(zed)

find_package(OpenCV REQUIRED)

aux_source_directory(. CODE)
add_executable(main ${CODE})

option(SHOWWHILE "show picure" OFF)

if(SHOWWHILE)
    message("this code will not show the save picure until you press the esc")
    add_definitions(-DSHOWWHILE)
endif()
include_directories(${OpenCV_INCLUDE_DIRS})

target_link_libraries(main ${OpenCV_LIBS})


