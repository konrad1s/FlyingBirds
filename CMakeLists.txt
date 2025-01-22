cmake_minimum_required(VERSION 3.10)
project(RpgGame LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_CXX_FLAGS_DEBUG "/Zi /MDd")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

option(BUILD_SHARED_LIBS "Build shared libraries" OFF)

include(FetchContent)
FetchContent_Declare(
    SFML
    GIT_REPOSITORY https://github.com/SFML/SFML.git
    GIT_TAG 2.6.x
    GIT_SHALLOW ON
    EXCLUDE_FROM_ALL
    SYSTEM
)
FetchContent_MakeAvailable(SFML)

FetchContent_Declare(
    lua
    GIT_REPOSITORY https://github.com/lua/lua.git
    GIT_TAG v5.4.7
    GIT_SHALLOW ON
)

FetchContent_Declare(
    sol2
    GIT_REPOSITORY https://github.com/ThePhD/sol2.git
    GIT_TAG v3.3.0
    GIT_SHALLOW ON
    EXCLUDE_FROM_ALL
    SYSTEM
)

FetchContent_MakeAvailable(lua sol2)
add_library(lua_lib STATIC)
target_include_directories(lua_lib PUBLIC ${lua_SOURCE_DIR})
file(GLOB SRC_FILES ${lua_SOURCE_DIR}/*.c)
list(REMOVE_ITEM SRC_FILES "${lua_SOURCE_DIR}/onelua.c")
list(REMOVE_ITEM SRC_FILES "${lua_SOURCE_DIR}/ltests.c")
message(STATUS "lua_src -> ${SRC_FILES}")
target_sources(lua_lib PRIVATE ${SRC_FILES})

target_link_libraries(lua_lib PRIVATE ${CMAKE_DL_LIBS})
if (UNIX)
    target_link_libraries(lua_lib PRIVATE m)
endif ()
add_library(antara::lua_lib ALIAS lua_lib)

add_subdirectory(proto)
add_subdirectory(Shared)
add_subdirectory(Server)
add_subdirectory(Client)
