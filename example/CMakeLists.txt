cmake_minimum_required(VERSION 3.13)

##############################################################################################################
find_package(fty-cmake PATHS ${CMAKE_BINARY_DIR}/fty-cmake)
##############################################################################################################

project(fty-cmake-rest-test
    VERSION 1.0.0
    DESCRIPTION "Helper for tntnet rest with cmake"
)

##############################################################################################################
set(CMAKE_LIBRARY_DIR "${CMAKE_INSTALL_PREFIX}/lib/bios")

etn_target(shared ${PROJECT_NAME}
    USES
        tntnet
        fty-cmake-rest
)

fty_ecppbuilder(
    TARGET ${PROJECT_NAME}
    WORKDIR src
        ECPP
            src/test.ecpp
)
