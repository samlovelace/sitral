
find_package(Protobuf REQUIRED)

set(PROTO_FILES
    ${CMAKE_CURRENT_SOURCE_DIR}/RegistryRequest.proto
)

protobuf_generate_cpp(PROTO_SRCS PROTO_HDRS
    ${PROTO_FILES}
)

add_library(sitral_proto OBJECT
    ${PROTO_SRCS}
    ${PROTO_HDRS}
)

target_include_directories(sitral_proto PUBLIC
    ${CMAKE_CURRENT_BINARY_DIR}
)

target_link_libraries(sitral_proto PUBLIC 
    ${Protobuf_LIBRARIES}
)