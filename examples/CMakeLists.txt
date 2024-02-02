include_directories("${CMAKE_CURRENT_SOURCE_DIR}/../src")

add_executable(Echo_Server echo-server.cpp)
target_link_libraries(Echo_Server PRIVATE "${CMAKE_BINARY_DIR}/src/SocketLib.lib")

add_executable(Simple_Client simple-client.cpp)
target_link_libraries(Simple_Client PRIVATE "${CMAKE_BINARY_DIR}/src/SocketLib.lib")

message(Include = "${CMAKE_CURRENT_SOURCE_DIR}/../src")
message(CMAKE_LIBRARY_OUTPUT_DIRECTORY="${CMAKE_LIBRARY_OUTPUT_DIRECTORY}")
message(CMAKE_CURRE="${CMAKE_LIBRARY_OUTPUT_DIRECTORY}")
message(CMAKE_OUT = "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/src")
message(CMAKE_OUT = "${CMAKE_BINARY_DIR}/src")
