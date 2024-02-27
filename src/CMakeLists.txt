find_package(Qt5 COMPONENTS Network Core REQUIRED)

add_library(SocketLib STATIC Socket.cpp socketqt.cpp)

target_link_libraries(SocketLib Qt5::Core Qt5::Network)
