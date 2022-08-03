cmake_minimum_required(VERSION 3.21)
project(project_kmean CXX)

find_package(OpenMP REQUIRED)
add_executable(project_kmean main.cpp miscellaneous/other.cpp)

# add libs you need
set(OpenCV_LIBS opencv_core opencv_imgproc opencv_highgui opencv_imgcodecs)

