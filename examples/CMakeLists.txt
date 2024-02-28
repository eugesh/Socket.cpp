find_package(Qt5 COMPONENTS Network Core REQUIRED)

SET(CMAKE_AUTOMOC ON)
SET(CMAKE_AUTOUIC ON)
SET(CMAKE_AUTORCC ON)

include_directories("${CMAKE_CURRENT_SOURCE_DIR}/../src")

#add_executable(Echo_Server echo-server.cpp)
if (UNIX)
    #target_link_libraries(Echo_Server PRIVATE "${SocketLib_BINARY_DIR}/src/libSocketLib.a" Qt5::Core Qt5::Network)
else ()
    #target_link_libraries(Echo_Server PRIVATE "${SocketLib_BINARY_DIR}/src/SocketLib.lib" Qt5::Core Qt5::Network)
endif ()

add_executable(Simple_Client simple-client.cpp)
if (UNIX)
    target_link_libraries(Simple_Client PRIVATE "${SocketLib_BINARY_DIR}/src/libSocketLib.a" Qt5::Core Qt5::Network)
else ()
    target_link_libraries(Simple_Client PRIVATE "${SocketLib_BINARY_DIR}/src/SocketLib.lib" Qt5::Core Qt5::Network)
endif ()

message(Include = "${CMAKE_CURRENT_SOURCE_DIR}/../src")
message(CMAKE_LIBRARY_OUTPUT_DIRECTORY="${CMAKE_LIBRARY_OUTPUT_DIRECTORY}")
message(CMAKE_CURRE="${CMAKE_LIBRARY_OUTPUT_DIRECTORY}")
message(CMAKE_OUT = "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/src")
message(CMAKE_OUT = "${CMAKE_BINARY_DIR}/src")
