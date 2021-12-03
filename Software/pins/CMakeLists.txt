
add_custom_command(
    PRE_BUILD
    OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/pins.h
    COMMAND ${CMAKE_CURRENT_SOURCE_DIR}/GeneratePinHeader.sh ${CMAKE_CURRENT_SOURCE_DIR}/pins.txt ${CMAKE_CURRENT_BINARY_DIR}/pins.h
    DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/pins.txt
)

add_library(pins INTERFACE)
add_dependencies(pins ${CMAKE_CURRENT_BINARY_DIR}/pins.h)
add_custom_target(generate_pins DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/pins.h)
target_include_directories(pins INTERFACE ${CMAKE_CURRENT_BINARY_DIR})

