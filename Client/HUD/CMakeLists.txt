add_library(ClientHUDs
    MenuHUD.cpp
    InGameHUD.cpp
    HUDManager.cpp
    GameEndHUD.cpp
)

target_include_directories(ClientHUDs PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_SOURCE_DIR}/Client
)

target_link_libraries(ClientHUDs PRIVATE
    ClientEntities ClientComponents
    Shared
    sfml-graphics sfml-window sfml-system
)
