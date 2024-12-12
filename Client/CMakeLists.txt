cmake_minimum_required(VERSION 3.16)

set(CLIENT_SOURCES
    main.cpp
    Client.cpp
    Game.cpp
    GameWorld.cpp
    ResourceManager.cpp
)

add_subdirectory(Entities)
add_subdirectory(Components)

add_executable(client
    ${CLIENT_SOURCES}
)

target_include_directories(client PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(client PUBLIC
    ClientEntities ClientComponents
    # shared
    # proto_generated
    sfml-system sfml-network sfml-graphics sfml-window
    ${Protobuf_LIBRARIES}
)
