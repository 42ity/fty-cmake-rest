find_program(Ecppc_EXECUTABLE NAMES ecppc)

macro(fty_ecppbuilder)
    cmake_parse_arguments(arg
        ""
        "TARGET;WORKDIR"
        "ECPP"
        ${ARGN}
    )

    if (NOT Ecppc_EXECUTABLE)
        message(FATAL_ERROR "Ecpp compiler was not found, please install `libtntnet-dev`")
    else()
        message("-- Ecpp compiler found: ${Ecppc_EXECUTABLE}")
    endif()

    add_dependencies(${arg_TARGET} tntnet)

    set(ecppc ${Ecppc_EXECUTABLE})

    if (arg_WORKDIR)
        get_filename_component(arg_WORKDIR ${arg_WORKDIR} ABSOLUTE)
    endif()

    foreach(ecppf ${arg_ECPP})
        get_filename_component(abs ${ecppf} ABSOLUTE)
        if (NOT arg_WORKDIR)
            get_filename_component(inc ${abs} DIRECTORY)
        else()
            set(inc ${arg_WORKDIR})
        endif()

        file(RELATIVE_PATH relPath ${inc} ${abs})
        get_filename_component(outDir ${relPath} DIRECTORY)
        get_filename_component(genName ${ecppf} NAME_WE)

        set(result ${CMAKE_CURRENT_BINARY_DIR}/${outDir}/${genName}.cpp)

        add_custom_command(
            OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/${outDir}/${genName}.cpp
            COMMAND ${ecppc} -I ${inc} ${abs} -o ${CMAKE_CURRENT_BINARY_DIR}/${outDir}/${genName}.cpp
            DEPENDS ${ecppf}
        )
        target_sources(${arg_TARGET} PRIVATE ${result})
        #target_sources(${arg_TARGET} PRIVATE ${ecppf})

    endforeach()

    message(STATUS "Compile ecpp for target ${arg_TARGET}")
endmacro()
