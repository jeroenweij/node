
set(CMAKE_TOOLCHAIN_FILE tools/cmake/ArduinoToolchain.cmake)

cmake_minimum_required(VERSION 2.8)

project(Node C CXX)

add_subdirectory(tools)
add_subdirectory(lib)

add_executable(${CMAKE_PROJECT_NAME} 
    src/Node.cpp
    )
set_target_properties(${CMAKE_PROJECT_NAME} PROPERTIES SUFFIX ".elf")
target_link_libraries(${CMAKE_PROJECT_NAME}
    arduinocore
    avrlib
    NodeLib
    arduinoServo
    )


