add_library(Game
    Game.cpp
    GameWorld.cpp
)

target_include_directories(Game PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(Game PUBLIC Entities 
    sfml-graphics 
    sfml-window 
    sfml-system
)
