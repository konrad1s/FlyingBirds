
cmake_minimum_required(VERSION 3.16)

set(SERVER_SOURCES
    main.cpp
    Server.cpp
    GameManager.cpp
    GameWorld.cpp
)

add_subdirectory(Entities)
add_subdirectory(Components)

add_executable(server
    ${SERVER_SOURCES}
)

target_include_directories(server PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(server PUBLIC
    ServerEntities ServerComponents
    Shared
    proto_generated
    sfml-system sfml-network sfml-graphics sfml-window
    ${Protobuf_LIBRARIES}
)

if(WIN32)
    add_custom_command(
        TARGET server
        COMMENT "Copy OpenAL DLL"
        PRE_BUILD COMMAND ${CMAKE_COMMAND} -E copy ${SFML_SOURCE_DIR}/extlibs/bin/$<IF:$<EQUAL:${CMAKE_SIZEOF_VOID_P},8>,x64,x86>/openal32.dll $<TARGET_FILE_DIR:server>
        VERBATIM)
endif()
