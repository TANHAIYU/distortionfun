cmake_minimum_required(VERSION 2.8) 
 
project( disfun )

find_package( OpenCV REQUIRED )

add_executable(disfun disfun.cpp)

target_link_libraries( disfun ${OpenCV_LIBS} )
