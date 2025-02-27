cmake_minimum_required(VERSION 3.16)

set(CLIENT_SOURCES
    main.cpp
    Client.cpp
    GameManager.cpp
    GameWorld.cpp
    ResourceManager.cpp
)

add_subdirectory(Entities)
add_subdirectory(Components)
add_subdirectory(Systems)
add_subdirectory(HUD)

add_executable(client
    ${CLIENT_SOURCES}
)

target_include_directories(client PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(client PUBLIC
    ClientEntities ClientComponents ClientSystems ClientHUDs
    Shared
    proto_generated
    sfml-system sfml-network sfml-graphics sfml-window
    ${Protobuf_LIBRARIES}
)

target_link_libraries(client PRIVATE embedded_assets)

if (MSVC)
    target_link_options(client PRIVATE "/WHOLEARCHIVE:embedded_assets")
else()
    target_link_options(client PRIVATE "-Wl,--whole-archive" embedded_assets "-Wl,--no-whole-archive")
endif()

if(WIN32)
    add_custom_command(
        TARGET client
        COMMENT "Copy OpenAL DLL"
        PRE_BUILD COMMAND ${CMAKE_COMMAND} -E copy ${SFML_SOURCE_DIR}/extlibs/bin/$<IF:$<EQUAL:${CMAKE_SIZEOF_VOID_P},8>,x64,x86>/openal32.dll $<TARGET_FILE_DIR:client>
        VERBATIM)
endif()
