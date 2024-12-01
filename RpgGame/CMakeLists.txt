cmake_minimum_required(VERSION 3.10)

project(RpgGame)

set(CMAKE_CXX_STANDARD 20)
set(SFML_STATIC_LIBRARIES TRUE)
add_compile_definitions(SFML_STATIC)

# Use SFML_DIR from the environment variable
set(SFML_DIR $ENV{SFML_DIR})

# Include SFML
find_package(SFML 2.6 REQUIRED COMPONENTS graphics window system)

# Add subdirectories
add_subdirectory(Client)
add_subdirectory(Common)
add_subdirectory(Resources)
add_subdirectory(Server)

option(BUILD_CLIENT "Build the Client application" ON)
option(BUILD_SERVER "Build the Server application" OFF)

add_executable(RpgGame "RpgGame.cpp" "RpgGame.h")
target_link_libraries(RpgGame PUBLIC Game 
    sfml-graphics 
    sfml-window 
    sfml-system
)