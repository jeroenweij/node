add_library(NodeLib
    id.h
    id.cpp
    Node.h
    Node.cpp
    NodeMaster.h
    NodeMaster.cpp
    IVariableHandler.h
    EPinMode.h
    EOperation.h
    EChannelId.h
    ErrorHandler.h
    ErrorHandler.cpp
)

target_include_directories(NodeLib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/..)

target_link_libraries(NodeLib
    arduinocore
    libtools
)
