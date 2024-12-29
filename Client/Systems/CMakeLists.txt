add_library(ClientSystems
    MovementSystem.cpp
)

target_include_directories(ClientSystems PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_SOURCE_DIR}/Client
)

target_link_libraries(ClientSystems PRIVATE
    client ClientComponents ClientEntities
    Shared
    proto_generated
    sfml-graphics sfml-window sfml-system
)
