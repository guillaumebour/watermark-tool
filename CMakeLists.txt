cmake_minimum_required(VERSION 2.8)
project( watermark )

set(watermark_VERSION_MAJOR 1)
set(watermark_VERSION_MINOR 0)

set(SOURCES
    ${PROJECT_SOURCE_DIR}/src/main.cpp
    ${PROJECT_SOURCE_DIR}/src/utils/log.cpp
    ${PROJECT_SOURCE_DIR}/src/utils/options.cpp
    ${PROJECT_SOURCE_DIR}/src/base/watermark.cpp
    )

find_package( OpenCV REQUIRED )
include_directories( ${OpenCV_INCLUDE_DIRS} )

include_directories(${PROJECT_SOURCE_DIR}/src)
add_executable( watermark-tool ${SOURCES})
target_link_libraries( watermark-tool ${OpenCV_LIBS} )