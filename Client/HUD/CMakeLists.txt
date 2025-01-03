add_library(ClientHUDs
    MenuHUD.cpp
)

target_include_directories(ClientHUDs PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_SOURCE_DIR}/Client
)

target_link_libraries(ClientHUDs PRIVATE
    client
    Shared
    sfml-graphics sfml-window sfml-system
)
