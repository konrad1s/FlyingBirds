add_library(ClientSystems
    MovementSystem.cpp
    ControlSystem.cpp
)

target_include_directories(ClientSystems PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${lua_SOURCE_DIR}
    ${sol2_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/Client
)

target_link_libraries(ClientSystems PRIVATE
    ClientComponents ClientEntities
    Shared
    proto_generated
    lua_lib
    sol2
    sfml-graphics sfml-window sfml-system
)
