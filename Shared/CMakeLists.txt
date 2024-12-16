add_library(Shared STATIC
    NetworkHandler.cpp
)

target_include_directories(Shared PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(Shared PUBLIC
    sfml-system sfml-network
    proto_generated
    ${Protobuf_LIBRARIES}
)

target_compile_features(Shared PUBLIC cxx_std_20)
