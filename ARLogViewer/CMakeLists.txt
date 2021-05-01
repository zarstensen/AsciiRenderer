project(ARLogViewer)

set(INCLUDE_FILES
    src/LogViewer.h
)

set(SRC_FILES
    src/LogViewer.cpp
    src/main.cpp
)

add_executable(${PROJECT_NAME}
    ${SRC_FILES}
    ${INCLUDE_FILES}
)

target_compile_definitions(${PROJECT_NAME} PUBLIC AR_LOG_APPEND)

target_link_libraries(${PROJECT_NAME} Asciir)


set_target_properties(${PROJECT_NAME}
    PROPERTIES
    OUTPUT_NAME ${PROJECT_NAME}
    RUNTIME_OUTPUT_DIRECTORY_DEBUG ${CMAKE_BINARY_DIR}/bin/Debug-${ARCH}/${PROJECT_NAME}
    RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_BINARY_DIR}/bin/Release-${ARCH}/${PROJECT_NAME}
)
