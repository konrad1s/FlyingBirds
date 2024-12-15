find_package(Protobuf REQUIRED)

set(PROTO_SRC_DIR ${CMAKE_CURRENT_BINARY_DIR}/generated)
set(PROTO_INCLUDE_DIR ${PROTO_SRC_DIR})

file(MAKE_DIRECTORY ${PROTO_SRC_DIR})

set(PROTO_FILES
    ${CMAKE_CURRENT_SOURCE_DIR}/network.proto
)

# Generate C++ sources from .proto files
protobuf_generate_cpp(PROTO_SRCS PROTO_HDRS ${PROTO_FILES}
    PROTOC_OUT_DIR ${PROTO_SRC_DIR}
)

# Create a library
add_library(proto_generated STATIC ${PROTO_SRCS})
target_include_directories(proto_generated PUBLIC ${PROTO_INCLUDE_DIR})
target_link_libraries(proto_generated PUBLIC protobuf::libprotobuf)
