include(CTest)

etn_test(${PROJECT_NAME}-test
    SOURCES
        main.cpp
    USES
        ${PROJECT_NAME}
        Catch2::Catch2
)
