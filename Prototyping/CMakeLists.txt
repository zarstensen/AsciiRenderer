project(Prototyping)

set(INCLUDE_FILES
    
)

set(SRC_FILES
    src/Executable.cpp
)

add_executable(${PROJECT_NAME}
    ${SRC_FILES}
    ${INCLUDE_FILES}
)

target_link_libraries(${PROJECT_NAME} Asciir)

message(STATUS ${CMAKE_BINARY_DIR})

set_target_properties(${PROJECT_NAME}
    PROPERTIES
    OUTPUT_NAME ${PROJECT_NAME}
    RUNTIME_OUTPUT_DIRECTORY_DEBUG ${CMAKE_BINARY_DIR}/bin/Debug-${ARCH}/${PROJECT_NAME}
    RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_BINARY_DIR}/bin/Release-${ARCH}/${PROJECT_NAME}
)
