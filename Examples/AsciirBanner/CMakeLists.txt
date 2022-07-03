project(AsciirBanner VERSION 1.0.0)

set(INCLUDE_FILES
    
)

set(SRC_FILES
    src/Banner.cpp
)

add_executable(${PROJECT_NAME}
    ${SRC_FILES}
    ${INCLUDE_FILES}
)

target_link_libraries(${PROJECT_NAME} Asciir)

set_target_properties(${PROJECT_NAME}
    PROPERTIES
    OUTPUT_NAME ${PROJECT_NAME}
    RUNTIME_OUTPUT_DIRECTORY_DEBUG ${CMAKE_BINARY_DIR}/bin/Debug-${ARCH}/${PROJECT_NAME}
    RUNTIME_OUTPUT_DIRECTORY_INLINEDEBUG ${CMAKE_BINARY_DIR}/bin/InlineDebug-${ARCH}/${PROJECT_NAME}
    RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_BINARY_DIR}/bin/Release-${ARCH}/${PROJECT_NAME}
    RUNTIME_OUTPUT_DIRECTORY_DEPLOYREL ${CMAKE_BINARY_DIR}/bin/DeployRelease-${ARCH}/${PROJECT_NAME}
)

# copy resources file to build destination
add_custom_command(TARGET ${PROJECT_NAME} PRE_BUILD COMMAND ${CMAKE_COMMAND} -E copy_directory "${CMAKE_CURRENT_SOURCE_DIR}/resources" "$<TARGET_FILE_DIR:${PROJECT_NAME}>/resources")
# for vs debugging
add_custom_command(TARGET ${PROJECT_NAME} PRE_BUILD COMMAND ${CMAKE_COMMAND} -E copy_directory "${CMAKE_CURRENT_SOURCE_DIR}/resources" "${CMAKE_CURRENT_BINARY_DIR}/resources")
