add_library(Entities
    Player.cpp
    Food.cpp
)

target_include_directories(Entities PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(Entities PUBLIC Components Resources 
    sfml-graphics 
    sfml-window 
    sfml-system
)
