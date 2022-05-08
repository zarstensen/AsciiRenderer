project(SuperAsciirBros)

set(INCLUDE_FILES
    include/Game.h
    include/GameLayer.h
    include/Background.h
    include/Ground.h
	include/Mario.h
)

set(SRC_FILES
    src/Game.cpp
    src/GameLayer.cpp
    src/Background.cpp
    src/Ground.cpp
	src/Mario.cpp
)

add_executable(${PROJECT_NAME}
    ${SRC_FILES}
    ${INCLUDE_FILES}
)

# for visual studio solution dir
add_custom_command(TARGET ${PROJECT_NAME} PRE_BUILD COMMAND ${CMAKE_COMMAND}
-E copy_directory "${CMAKE_CURRENT_SOURCE_DIR}/resources" "${CMAKE_CURRENT_BINARY_DIR}/resources")

# for the final executable
add_custom_command(TARGET ${PROJECT_NAME} PRE_BUILD COMMAND ${CMAKE_COMMAND}
-E copy_directory "${CMAKE_CURRENT_SOURCE_DIR}/resources" "$<TARGET_FILE_DIR:${PROJECT_NAME}>/resources")

target_include_directories(${PROJECT_NAME} PRIVATE include)

target_link_libraries(${PROJECT_NAME} Asciir)

message(STATUS ${CMAKE_BINARY_DIR})

set_target_properties(${PROJECT_NAME}
    PROPERTIES
    OUTPUT_NAME ${PROJECT_NAME}
    RUNTIME_OUTPUT_DIRECTORY_DEBUG ${CMAKE_BINARY_DIR}/bin/Debug-${ARCH}/${PROJECT_NAME}
    RUNTIME_OUTPUT_DIRECTORY_INLINEDEBUG ${CMAKE_BINARY_DIR}/bin/InlineDebug-${ARCH}/${PROJECT_NAME}
    RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_BINARY_DIR}/bin/Release-${ARCH}/${PROJECT_NAME}
    RUNTIME_OUTPUT_DIRECTORY_DEPLOYREL ${CMAKE_BINARY_DIR}/bin/DeployRelease-${ARCH}/${PROJECT_NAME}
)
