cmake_minimum_required(VERSION 2.8)

project(camera_calibration)
add_executable(${PROJECT_NAME} "main.cpp")

set(OpenCV_DIR "/home/ccc/opencv-2.4.10/build/")
find_package( OpenCV REQUIRED )
include_directories( ${OpenCV_INCLUDE_DIRS} )
target_link_libraries( camera_calibration ${OpenCV_LIBS} )

