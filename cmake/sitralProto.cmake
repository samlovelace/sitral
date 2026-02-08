function(sitral_generate_proto)
    set(oneValueArgs TARGET)
    set(multiValueArgs PROTOS)

    cmake_parse_arguments(ARGS "" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})

    if(NOT TARGET ${ARGS_TARGET})
        message(FATAL_ERROR
            "sitral_generate_proto: target '${ARGS_TARGET}' does not exist"
        )
    endif()

    find_package(Protobuf REQUIRED)

    get_target_property(target_dir ${ARGS_TARGET} BINARY_DIR)
    set(proto_out_dir ${target_dir}/proto)
    file(MAKE_DIRECTORY ${proto_out_dir})

    set(generated_srcs)
    set(generated_hdrs)

    foreach(proto ${ARGS_PROTOS})
        get_filename_component(proto_name ${proto} NAME_WE)
        get_filename_component(proto_dir  ${proto} DIRECTORY)

        list(APPEND generated_srcs ${proto_out_dir}/${proto_name}.pb.cc)
        list(APPEND generated_hdrs ${proto_out_dir}/${proto_name}.pb.h)

        add_custom_command(
            OUTPUT
                ${proto_out_dir}/${proto_name}.pb.cc
                ${proto_out_dir}/${proto_name}.pb.h
            COMMAND protobuf::protoc
            ARGS
                --cpp_out=${proto_out_dir}
                -I ${proto_dir}
                ${proto}
            DEPENDS ${proto}
            VERBATIM
        )
    endforeach()

    target_sources(${ARGS_TARGET}
        PRIVATE ${generated_srcs} ${generated_hdrs}
    )

    target_include_directories(${ARGS_TARGET}
        PRIVATE ${proto_out_dir}
    )

    target_link_libraries(${ARGS_TARGET}
        PRIVATE protobuf::libprotobuf
    )
endfunction()
