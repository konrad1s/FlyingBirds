cmake_minimum_required(VERSION 3.10)
project(RpgGame LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# find_package(Protobuf REQUIRED)
find_package(SFML 2.5 COMPONENTS system window graphics network REQUIRED)

# Provide these to subdirectories
include_directories(${Protobuf_INCLUDE_DIRS})

# Add subdirectories
# add_subdirectory(proto)
add_subdirectory(Shared)
add_subdirectory(Server)
add_subdirectory(Client)
