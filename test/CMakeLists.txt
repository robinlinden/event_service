project(test_event_service)

add_executable(${PROJECT_NAME}
    src/main.cpp
)

add_test(${PROJECT_NAME} ${PROJECT_NAME})

target_link_libraries(${PROJECT_NAME}
    PRIVATE
        event_service
)
