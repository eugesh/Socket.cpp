include_directories("${CMAKE_CURRENT_SOURCE_DIR}")

add_library(SocketLib STATIC Socket.cpp)
	
message(CMAKE_CURRENT_SOURCE_DIR = "${CMAKE_CURRENT_SOURCE_DIR}")
message(SocketLib_SOURCE_DIR = "${SocketLib_SOURCE_DIR}")
