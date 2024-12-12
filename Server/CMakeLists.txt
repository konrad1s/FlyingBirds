
cmake_minimum_required(VERSION 3.16)

set(SERVER_SOURCES
    main.cpp
    Server.cpp
    GameManager.cpp
    GameWorld.cpp
)

add_subdirectory(Entities)
add_subdirectory(Components)

add_executable(server
    ${SERVER_SOURCES}
)

target_include_directories(server PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(server PUBLIC
    ServerEntities ServerComponents
    # shared
    # proto_generated
    sfml-system sfml-network sfml-graphics sfml-window
    ${Protobuf_LIBRARIES}
)
