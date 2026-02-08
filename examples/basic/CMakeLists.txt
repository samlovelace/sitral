cmake_minimum_required(VERSION 3.16)
project(basic_ex LANGUAGES CXX)

find_package(sitral REQUIRED)

add_executable(publisher publisher.cpp)
add_executable(subscriber subscriber.cpp)

sitral_generate_proto(
    TARGET publisher
    PROTOS
        ${CMAKE_CURRENT_LIST_DIR}/proto/Test.proto
)

sitral_generate_proto(
    TARGET subscriber
    PROTOS
        ${CMAKE_CURRENT_LIST_DIR}/proto/Test.proto
)

target_link_libraries(publisher PRIVATE sitral::sitral)
target_link_libraries(subscriber PRIVATE sitral::sitral)