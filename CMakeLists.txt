cmake_minimum_required(VERSION 3.10)
project(FlyingBirds LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_CXX_FLAGS_DEBUG "/Zi /MDd")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

option(BUILD_SHARED_LIBS "Build shared libraries" OFF)

# --------------------------------------------------------------------
# 1. Download dependencies via FetchContent
# --------------------------------------------------------------------
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

# --------------------------------------------------------------------
# 2. Build the lua_lib static library
# --------------------------------------------------------------------
add_library(lua_lib STATIC)
target_include_directories(lua_lib PUBLIC ${lua_SOURCE_DIR})
file(GLOB SRC_FILES ${lua_SOURCE_DIR}/*.c)
list(REMOVE_ITEM SRC_FILES "${lua_SOURCE_DIR}/onelua.c")
list(REMOVE_ITEM SRC_FILES "${lua_SOURCE_DIR}/ltests.c")
message(STATUS "Lua source files -> ${SRC_FILES}")
target_sources(lua_lib PRIVATE ${SRC_FILES})
target_link_libraries(lua_lib PRIVATE ${CMAKE_DL_LIBS})
if (UNIX)
    target_link_libraries(lua_lib PRIVATE m)
endif()
add_library(antara::lua_lib ALIAS lua_lib)

# --------------------------------------------------------------------
# 3. Embed asset files (xxd -> .cpp) into a static library
# --------------------------------------------------------------------
file(GLOB_RECURSE EMBED_ASSET_FILES
    "${CMAKE_CURRENT_SOURCE_DIR}/assets/*.png"
    "${CMAKE_CURRENT_SOURCE_DIR}/assets/*.ttf"
    "${CMAKE_CURRENT_SOURCE_DIR}/assets/*.frag"
)

set(EMBEDDED_ASSET_CPP_LIST "")

foreach(ASSET_FILE ${EMBED_ASSET_FILES})
    file(RELATIVE_PATH ASSET_REL_PATH "${CMAKE_CURRENT_SOURCE_DIR}/assets" "${ASSET_FILE}")

    # Convert slashes, dots, and hyphens into underscores for the symbol name
    string(REPLACE "/" "_" ASSET_OUT_NAME "${ASSET_REL_PATH}")
    string(REPLACE "." "_" ASSET_OUT_NAME "${ASSET_OUT_NAME}")
    string(REPLACE "-" "_" ASSET_OUT_NAME "${ASSET_OUT_NAME}")

    # Generate a .cpp file in the build directory
    set(ASSET_CPP "${CMAKE_CURRENT_BINARY_DIR}/embedded_assets/${ASSET_OUT_NAME}.cpp")

    add_custom_command(
        OUTPUT "${ASSET_CPP}"
        COMMAND ${CMAKE_COMMAND} -E make_directory "${CMAKE_CURRENT_BINARY_DIR}/embedded_assets"
        COMMAND xxd -i -n ${ASSET_OUT_NAME} "${ASSET_FILE}" > "${ASSET_CPP}"
        COMMAND ${CMAKE_COMMAND}
                -DOUT_CPP=${ASSET_CPP}
                -DASSET_OUT_NAME=${ASSET_OUT_NAME}
                -DREL_PATH=${ASSET_REL_PATH}
                -P "${CMAKE_CURRENT_SOURCE_DIR}/EmbedRegister.cmake"
        DEPENDS "${ASSET_FILE}"
        COMMENT "Embedding ${ASSET_FILE} -> ${ASSET_CPP}"
        VERBATIM
    )

    list(APPEND EMBEDDED_ASSET_CPP_LIST "${ASSET_CPP}")
endforeach()

add_library(embedded_assets STATIC ${EMBEDDED_ASSET_CPP_LIST})

target_include_directories(embedded_assets PRIVATE
    "${CMAKE_CURRENT_SOURCE_DIR}/Shared"
)

# --------------------------------------------------------------------
# 4. Add subdirectories
# --------------------------------------------------------------------
add_subdirectory(proto)
add_subdirectory(Shared)
add_subdirectory(Server)
add_subdirectory(Client)
