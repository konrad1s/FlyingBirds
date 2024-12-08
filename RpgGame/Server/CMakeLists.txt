add_library(Server
    Server.cpp
    GameManager.cpp
)

target_include_directories(Server PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(Server PUBLIC
    sfml-graphics
    sfml-window
    sfml-system
    sfml-network
)