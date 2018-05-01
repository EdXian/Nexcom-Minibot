cmake_minimum_required (VERSION 3.8)
project(kalmanfilter)
set(CMAKE_CONFIGURATION_TYPES "Release")


include_directories(
  ../
  ../Eigen
)

add_library(kalmanfilter "kalmanFilter.cpp" "kalmanFilter.h" )

SET(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR}/bin)