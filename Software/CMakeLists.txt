
include(tools/cmake/arduino.cmake)

cmake_minimum_required(VERSION 2.8)

project(Node C CXX)
#add_compile_definitions(DEBUG)

add_subdirectory(tools)
add_subdirectory(lib)
add_subdirectory(pins)

add_executable(${CMAKE_PROJECT_NAME} 
    src/NodeSketch.cpp
    src/Channel.h
    src/Channel.cpp
    src/ServoChannel.h
    src/ServoChannel.cpp
    src/ChannelManager.h
    src/ChannelManager.cpp
)
set_target_properties(${CMAKE_PROJECT_NAME} PROPERTIES SUFFIX ".elf")

target_link_libraries(${CMAKE_PROJECT_NAME}
    arduinocore
    avrlib
    pins
    NodeLib
    libservo
)

add_dependencies(${CMAKE_PROJECT_NAME} generate_pins)

