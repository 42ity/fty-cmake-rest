cmake_minimum_required(VERSION 3.13)

##############################################################################################################

project(fty-cmake-rest
    VERSION 1.0.0
    DESCRIPTION "Helper for tntnet rest with cmake"
)

##############################################################################################################

find_package(fty-cmake PATHS ${CMAKE_BINARY_DIR}/fty-cmake)

##############################################################################################################

etn_target(static ${PROJECT_NAME}
    PUBLIC
        fty/rest-support.h
        fty/error-string.h
        fty/audit-log.h

        fty/rest/translate.h
        fty/rest/runner.h

        src/runner.cpp
    CMAKE
        cmake/ecppbuilder.cmake
    USES_PUBLIC
        fty-utils
        fty-pack
)

##############################################################################################################

if (BUILD_TESTING)
    include(cmake/ecppbuilder.cmake)
    add_subdirectory(example)
    add_subdirectory(test)
endif()
