cmake_minimum_required(VERSION 2.6)
project(vcd_diff)
include_directories("include")
add_library(vcd_util 
	src/VCDparse.cpp 
	src/ValChange.cpp 
	src/diff.cpp 
	include/g_func.h)
add_executable(vcd_diff src/SourceMain.cpp)
target_link_libraries(vcd_diff vcd_util)
