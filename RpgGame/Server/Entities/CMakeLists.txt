add_library(Entities
    Player.cpp
)

target_include_directories(Entities PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(Entities PUBLIC Components 
    sfml-graphics 
    sfml-window 
    sfml-system
)
