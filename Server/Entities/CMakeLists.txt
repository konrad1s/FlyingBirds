add_library(ServerEntities
    Player.cpp
)

target_include_directories(ServerEntities PUBLIC 
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_SOURCE_DIR}/Server
)

target_link_libraries(ServerEntities PRIVATE
    ServerComponents Shared server
    sfml-graphics sfml-window sfml-system
)
