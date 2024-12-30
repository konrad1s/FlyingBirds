add_library(ServerSystems
    MovementSystem.cpp
    CollisionSystem.cpp
)

target_include_directories(ServerSystems PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_SOURCE_DIR}/Server
)

target_link_libraries(ServerSystems PRIVATE
    server ServerComponents ServerEntities
    Shared
    proto_generated
    sfml-graphics sfml-window sfml-system
)
