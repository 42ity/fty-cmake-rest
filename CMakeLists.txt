cmake_minimum_required(VERSION 3.13)

##############################################################################################################

project(fty-cmake-rest
    VERSION 1.0.0
    DESCRIPTION "Helper for tntnet rest with cmake"
)

##############################################################################################################

find_package(fty-cmake PATHS ${CMAKE_BINARY_DIR}/fty-cmake)

##############################################################################################################

etn_target(shared ${PROJECT_NAME}
    PUBLIC
        fty/rest/audit-log.h
        fty/rest/component.h
        fty/rest/error.h
        fty/rest/runner.h
        fty/rest/translate.h
        fty/rest/reply.h
        fty/rest/request.h
        fty/rest/user.h

        src/runner.cpp
        src/reply.cpp
        src/request.cpp
        src/user.cpp
    CMAKE
        cmake/ecppbuilder.cmake
    USES_PRIVATE
        fty_common_rest
    USES_PUBLIC
        fty-utils
        fty-pack
)

##############################################################################################################

if (BUILD_TESTING)
    #include(cmake/ecppbuilder.cmake)
    #add_subdirectory(example)
    add_subdirectory(test)
endif()
