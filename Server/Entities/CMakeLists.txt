add_library(ServerEntities
    Player.cpp
    Food.cpp
)

target_include_directories(ServerEntities PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(ServerEntities PRIVATE
    ServerComponents
    sfml-graphics sfml-window sfml-system
)
