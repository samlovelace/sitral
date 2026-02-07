cmake_minimum_required(VERSION 3.16)
project(basic_ex LANGUAGES CXX)

find_package(Protobuf REQUIRED)
message(STATUS "Using Protobuf ${Protobuf_VERSION}")

add_subdirectory(../../proto proto)

set(SOURCES
    ${CMAKE_CURRENT_LIST_DIR}/../../src/BroadcastServer.cpp
    ${CMAKE_CURRENT_LIST_DIR}/../../src/ClientSocket.cpp
    ${CMAKE_CURRENT_LIST_DIR}/../../src/ServerSocket.cpp
    ${CMAKE_CURRENT_LIST_DIR}/../../src/Registry.cpp
)

set(PROTO_SOURCES
    ${CMAKE_CURRENT_LIST_DIR}/../../proto/example.pb.cc
)

add_executable(registry registry.cpp ${SOURCES})
target_include_directories(registry PRIVATE ../../include)
target_link_libraries(registry PRIVATE sitral_proto)

add_executable(publisher publisher.cpp ${SOURCES})
target_include_directories(publisher PRIVATE ../../include)
target_link_libraries(publisher PRIVATE sitral_proto)

add_executable(subscriber subscriber.cpp ${SOURCES})
target_include_directories(subscriber PRIVATE ../../include)
target_link_libraries(subscriber PRIVATE sitral_proto)