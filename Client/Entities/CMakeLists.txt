add_library(ClientEntities
    Player.cpp
    Food.cpp
    SpeedBoost.cpp
    EntityFactory.cpp
    Protection.cpp
)

target_include_directories(ClientEntities PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_SOURCE_DIR}/Client
)

target_link_libraries(ClientEntities PRIVATE
    client ClientComponents
    Shared
    sfml-graphics sfml-window sfml-system
)
