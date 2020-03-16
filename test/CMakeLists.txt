project(test_event_service)

add_executable(${PROJECT_NAME}
    src/main.cpp
)

add_test(${PROJECT_NAME} ${PROJECT_NAME})

target_link_libraries(${PROJECT_NAME}
    PRIVATE
        event_service
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
