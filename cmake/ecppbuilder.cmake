
macro(fty_ecppbuilder)
    cmake_parse_arguments(arg
        ""
        "TARGET;WORKDIR"
        "ECPP"
        ${ARGN}
    )

    if (NOT Ecppc_EXECUTABLE)
        find_program(Ecppc_EXECUTABLE NAMES ecppc PATHS ${CMAKE_BINARY_DIR}/deps-runtime/bin)
        #message(FATAL_ERROR "Ecpp compiler was not found, please install `libtntnet-dev`")
    else()
        message("-- Ecpp compiler found: ${Ecppc_EXECUTABLE}")
    endif()

    add_dependencies(${arg_TARGET} tntnet)

    if (arg_WORKDIR)
        get_filename_component(arg_WORKDIR ${arg_WORKDIR} ABSOLUTE)
    endif()

    foreach(ecppf ${arg_ECPP})
        get_filename_component(abs ${ecppf} ABSOLUTE)
        file(RELATIVE_PATH relPath ${CMAKE_CURRENT_SOURCE_DIR} ${abs})

        if (NOT arg_WORKDIR)
            get_filename_component(inc ${abs} DIRECTORY)
        else()
            set(inc ${arg_WORKDIR})
        endif()

        get_filename_component(outDir ${relPath} DIRECTORY)
        get_filename_component(genName ${ecppf} NAME_WE)

        set(result ${CMAKE_CURRENT_BINARY_DIR}/${outDir}/${genName}.cpp)
        file(MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/${outDir})

        add_custom_command(
            OUTPUT ${result}
            COMMAND ${Ecppc_EXECUTABLE} -I ${inc} ${abs} -o ${result}
            COMMAND ${FTY_CMAKE_REST_CMAKE_DIR}/cmake/disable-logtrace.sh ${result}
            DEPENDS ${ecppf} ${Ecppc_EXECUTABLE}
        )
        target_sources(${arg_TARGET} PRIVATE ${result})
        target_sources(${arg_TARGET} PRIVATE ${ecppf})

    endforeach()

    message(STATUS "Compile ecpp for target ${arg_TARGET}")
endmacro()
