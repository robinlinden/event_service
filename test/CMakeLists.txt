project(test_event_service)

include(FetchContent)
include(GoogleTest)

FetchContent_Declare(googletest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG 703bd9caab50b139428cea1aaff9974ebee5742e # v1.10.0
)

FetchContent_MakeAvailable(googletest)

add_executable(${PROJECT_NAME}
    src/main.cpp
    src/test_event_service.cpp
)

target_link_libraries(${PROJECT_NAME}
    PRIVATE
        event_service
        gmock
)

target_compile_features(${PROJECT_NAME}
    PRIVATE
        cxx_std_17
)

set_target_properties(${PROJECT_NAME}
    PROPERTIES
        CXX_STANDARD 17
        CXX_STANDARD_REQUIRED YES
        CXX_EXTENSIONS NO
)

gtest_discover_tests(${PROJECT_NAME})
