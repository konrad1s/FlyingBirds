add_library(Resources
    ResourceManager.cpp
)

target_include_directories(Resources PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(Resources PUBLIC 
    sfml-graphics 
    sfml-window 
    sfml-system
)