add_library(NodeLib
    NodeLib/id.h
    NodeLib/id.cpp
    NodeLib/NodeComm.h
    NodeLib/NodeComm.cpp
    NodeLib/NodeMaster.h
    NodeLib/NodeMaster.cpp
    NodeLib/IVariableHandler.h
)

target_include_directories(NodeLib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(NodeLib
    arduinocore
    )
