add_library(ClientEntities
    Player.cpp
    Food.cpp
)

target_include_directories(ClientEntities PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_SOURCE_DIR}/Client
)

target_link_libraries(ClientEntities PRIVATE
    ClientComponents
    sfml-graphics sfml-window sfml-system
)
