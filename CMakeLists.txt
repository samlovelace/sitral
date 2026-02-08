cmake_minimum_required(VERSION 3.1)
project(sitral)

find_package(Protobuf REQUIRED)
message(STATUS "Using Protobuf ${Protobuf_VERSION}")

add_subdirectory(proto)

set(SOURCES 
    ${CMAKE_CURRENT_LIST_DIR}/src/BroadcastServer.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/ClientSocket.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/Registry.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/ServerSocket.cpp
)

add_library(${PROJECT_NAME} 
    ${SOURCES}
    $<TARGET_OBJECTS:sitral_proto>
)
add_library(sitral::sitral ALIAS sitral) 
target_link_libraries(${PROJECT_NAME} PUBLIC protobuf::libprotobuf)

target_include_directories(${PROJECT_NAME}
    PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/include>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}/proto>
    $<INSTALL_INTERFACE:include>
)

# registry executable 
add_executable(sitral-registry src/registry_main.cpp ${SOURCES})
target_include_directories(sitral-registry PRIVATE ${CMAKE_CURRENT_LIST_DIR}/include)
target_link_libraries(sitral-registry PRIVATE sitral)

include(GNUInstallDirs)

install(
    TARGETS ${PROJECT_NAME}
    EXPORT ${PROJECT_NAME}Targets
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

install(
    DIRECTORY include/
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)

install(
    FILES proto/RegistryRequest.proto
    DESTINATION ${CMAKE_INSTALL_DATADIR}/sitral/proto
)

install(
    DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/proto/
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/sitral/proto
    FILES_MATCHING PATTERN "*.pb.h"
)

# install registry exe
install(
    TARGETS sitral-registry
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

# export targets 
install(
    EXPORT ${PROJECT_NAME}Targets
    FILE ${PROJECT_NAME}Targets.cmake
    NAMESPACE sitral::
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/sitral
)

include(CMakePackageConfigHelpers)

configure_package_config_file(
    cmake/sitralConfig.cmake.in
    ${CMAKE_CURRENT_BINARY_DIR}/sitralConfig.cmake
    INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/sitral
)

install(
    FILES ${CMAKE_CURRENT_BINARY_DIR}/sitralConfig.cmake
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/sitral
)

install(
    FILES cmake/sitralProto.cmake
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/sitral
)
