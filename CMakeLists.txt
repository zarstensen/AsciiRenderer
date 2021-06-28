cmake_minimum_required(VERSION 3.0.0)
Project(Asciir)

set(CMAKE_CXX_STANDARD 20)

# place all predefined target projects into a filter
set_property(GLOBAL PROPERTY USE_FOLDERS ON)
set(PREDEFINED_TARGETS_FOLDER "CustomTargets")

if(${MSVC})

set(CMAKE_CONFIGURATION_TYPES "Debug;Release;InlineDebug;DeployRel" CACHE STRING "" FORCE)

set(CMAKE_CXX_FLAGS_INLINEDEBUG         "${CMAKE_CXX_FLAGS_DEBUG} /Ob2" CACHE STRING "" FORCE)
set(CMAKE_EXE_FLAGS_INLINEDEBUG         "${CMAKE_EXE_FLAGS_DEBUG}" CACHE STRING "" FORCE)
set(CMAKE_LINKER_FLAGS_INLINEDEBUG      "${CMAKE_LINKER_FLAGS_DEBUG}" CACHE STRING "" FORCE)
set(CMAKE_EXE_LINKER_FLAGS_INLINEDEBUG  "${CMAKE_EXE_LINKER_FLAGS_DEBUG}" CACHE STRING "" FORCE)

set(CMAKE_CXX_FLAGS_DEPLOYREL        "${CMAKE_CXX_FLAGS_RELEASE}" CACHE STRING "" FORCE)
set(CMAKE_EXE_FLAGS_DEPLOYREL         "${CMAKE_EXE_FLAGS_RELEASE}" CACHE STRING "" FORCE)
set(CMAKE_LINKER_FLAGS_DEPLOYREL      "${CMAKE_LINKER_FLAGS_RELEASE}" CACHE STRING "" FORCE)
set(CMAKE_EXE_LINKER_FLAGS_DEPLOYREL  "${CMAKE_EXE_LINKER_FLAGS_RELEASE}" CACHE STRING "" FORCE)

endif()

# options
option(ASCIIR_LOG_VIEWER "builds an executable to view log files" ON)
option(ASCIIR_EXAMPLES "build example projects" OFF)
option(ASCIIR_HIGH_PRECISSION_FLOAT "uses double instead of float as floating point data type (uses more memory)" OFF)

if(CMAKE_SIZEOF_VOID_P EQUAL 8)
set(ARCH "x64")
elseif(CMAKE_SIZEOF_VOID_P EQUAL 4)
set(ARCH "x86")
else()
message(STATUS "unknown architecture")
endif()

define_property(
    TARGET
    PROPERTY FOLDER
    INHERITED
    BRIEF_DOCS "Set the folder name."
    FULL_DOCS  "Use to organize targets in an IDE."
)

set(EXAMPLES
    BasicInputExample
    MouseInputExample
    RendererExample
    TriangleExample
)

set(INCLUDE_DIRS
    src/pch
    src
)

set(SRC_DIR_CORE  
    src/Asciir/Core/Engine.cpp
    src/Asciir/Core/Layer.cpp 
    src/Asciir/Core/LayerStack.cpp 
    src/Asciir/Core/Terminal.cpp 
    src/Asciir/Core/Timing.cpp  
    src/Asciir/Core/AsciirLiterals.cpp
)

set(HEADER_DIR_CORE 
    src/Asciir/Core/Core.h 
    src/Asciir/Core/Engine.h 
    src/Asciir/Core/EntryPoint.h 
    src/Asciir/Core/Layer.h 
    src/Asciir/Core/LayerStack.h 
    src/Asciir/Core/MacroArguments.h
    src/Asciir/Core/Terminal.h 
    src/Asciir/Core/Timing.h
    src/Asciir/Core/AsciirLiterals.h
)

set(DIR_EVENT
    src/Asciir/Event/Event.h
    src/Asciir/Event/KeyEvent.h
    src/Asciir/Event/MouseEvent.h
    src/Asciir/Event/TerminalEvent.h
)


set(DIR_INPUT
    src/Asciir/Input/Input.h
    src/Asciir/Input/KeyCodes.h
    src/Asciir/Input/EventListener.h
)

set(SRC_DIR_LOGGING
    src/Asciir/Logging/FileLog.cpp 
    src/Asciir/Logging/FileLog.ipp
    src/Asciir/Logging/ErrLog.ipp  
    src/Asciir/Logging/Log.cpp
)

set(HEADER_DIR_LOGGING
    src/Asciir/Logging/FileLog.h
    src/Asciir/Logging/Log.h
)

set(SRC_DIR_MATH
    src/Asciir/Math/Math.cpp
    src/Asciir/Math/Lines.cpp
    src/Asciir/Math/Lines.ipp
    src/Asciir/Math/Vertices.ipp
    src/Asciir/Math/Vector.ipp
    src/Asciir/Math/Matrix.ipp
    src/Asciir/Math/Tensor.ipp
)

set(HEADER_DIR_MATH
    src/Asciir/Math/Math.h
    src/Asciir/Math/Lines.h
    src/Asciir/Math/Vertices.h
    src/Asciir/Math/Vector.h
    src/Asciir/Math/Matrix.h
    src/Asciir/Math/Tensor.h
)

set(SRC_DIR_RENDERING
    src/Asciir/Rendering/AsciiAttributes.cpp 
    src/Asciir/Rendering/Mesh.cpp
    src/Asciir/Rendering/TerminalRenderer.cpp
    src/Asciir/Rendering/Renderer.cpp
    src/Asciir/Rendering/Renderer.ipp
)

set(HEADER_DIR_RENDERING
    src/Asciir/Rendering/AsciiAttributes.h
    src/Asciir/Rendering/Mesh.h
    src/Asciir/Rendering/TerminalRenderer.h
    src/Asciir/Rendering/Renderer.h
    src/Asciir/Rendering/RenderConsts.h
)

if(WIN32)

set(SRC_DIR_PLATFORM
    # core
    src/Asciir/Platform/Windows/WinInput.cpp

    # rendering
    src/Asciir/Platform/Windows/WindowsARAttributes.cpp

    # input
    src/Asciir/Platform/Windows/WinEventListener.cpp
)

set(HEADER_DIR_PLATFORM
    # core
    src/Asciir/Platform/Windows/WinInput.h
    src/Asciir/Platform/Windows/KeyCodeMap.h
    
    #rendering
    src/Asciir/Platform/Windows/WindowsARAttributes.h

    # input
    src/Asciir/Platform/Windows/WinEventListener.h
)

elseif(UNIX)

set(SRC_DIR_PLATFORM
    # core
    src/Asciir/Platform/Unix/UnixInput.cpp

    # rendering
    src/Asciir/Platform/Unix/UnixARAttributes.cpp
)

set(HEADER_DIR_PLATFORM
    # core
    src/Asciir/Platform/Unix/UnixInput.h
    src/Asciir/Platform/Unix/KeyCodeMap.h

    #rendering
    src/Asciir/Platform/Unix/UnixARAttributes.h
)

else()
message(SEND_ERROR "Platform not supported")
endif()

add_library(${PROJECT_NAME} STATIC
    src/Asciir.h
    src/AsciirHeaders.h
    src/pch/arpch.h
    src/pch/arpch.cpp

    src/Asciir/Platform/PlatformMacros.h

    ${HEADER_DIR_CORE}
    ${SRC_DIR_CORE}

    ${DIR_EVENT}

    ${DIR_INPUT}

    ${HEADER_DIR_LOGGING}
    ${SRC_DIR_LOGGING}

    ${HEADER_DIR_MATH}
    ${SRC_DIR_MATH}

    ${HEADER_DIR_RENDERING}
    ${SRC_DIR_RENDERING}

    ${SRC_DIR_PLATFORM}
    ${HEADER_DIR_PLATFORM}
)

source_group("core/header"        FILES ${HEADER_DIR_CORE})
source_group("core/src"           FILES ${SRC_DIR_CORE})

source_group("event"              FILES ${DIR_EVENT})

source_group("input"              FILES ${DIR_INPUT})

source_group("logging/header"     FILES ${HEADER_DIR_LOGGING})
source_group("logging/src"        FILES ${SRC_DIR_LOGGING})

source_group("math/header"        FILES ${HEADER_DIR_MATH})
source_group("math/src"           FILES ${SRC_DIR_MATH})

source_group("rendering/header"   FILES ${HEADER_DIR_RENDERING})
source_group("rendering/src"      FILES ${SRC_DIR_RENDERING})

source_group("platform/"           FILES src/Asciir/Platform/PlatformMacros.h)

if(WIN32)
source_group("platform/windows/header"  FILES ${HEADER_DIR_PLATFORM})
source_group("platform/windows/src"  FILES ${SRC_DIR_PLATFORM})
endif()

target_include_directories(${PROJECT_NAME} PUBLIC ${INCLUDE_DIRS})

target_compile_definitions(${PROJECT_NAME} PUBLIC
$<$<CONFIG:DEBUG,INLINEDEBUG>:AR_DEBUG>
$<$<CONFIG:RELEASE>:AR_SAFE_RELEASE AR_CORE_VERBOSITY=2 AR_CLIENT_VERBOSITY=4>
$<$<CONFIG:RELEASE,DEBUG,INLINEDEBUG>:AR_CORE_LOG_DIR="../../../logs/core_log.log" AR_CLIENT_LOG_DIR="../../../logs/client_log.log">
$<$<CONFIG:DEPLOYREL>:AR_CORE_LOG_DIR="./logs/core_log.log" AR_CLIENT_LOG_DIR="./logs/client_log.log">
$<$<BOOL:${ASCIIR_HIGH_PRECISSION_FLOAT}>:AR_HIGH_PRECISSION_FLOAT>)

if(${UNIX})
find_package(X11 REQUIRED)
include_directories(${X11_INCLUDE_DIRS})
target_link_libraries(${PROJECT_NAME} PUBLIC ${X11_LIBRARIES})

find_package(Threads REQUIRED)
include_directories(${THREADS_INCLUDE_DIRS})
target_link_libraries(${PROJECT_NAME} PUBLIC Threads::Threads)
endif()

set_target_properties(${PROJECT_NAME}
    PROPERTIES
    OUTPUT_NAME ${PROJECT_NAME}
    ARCHIVE_OUTPUT_DIRECTORY_DEBUG ${CMAKE_BINARY_DIR}/bin/Debug-${ARCH}
    ARCHIVE_OUTPUT_DIRECTORY_INLINEDEBUG ${CMAKE_BINARY_DIR}/bin/InlineDebug-${ARCH}
    ARCHIVE_OUTPUT_DIRECTORY_RELEASE ${CMAKE_BINARY_DIR}/bin/Release-${ARCH}
    ARCHIVE_OUTPUT_DIRECTORY_DEPLOYREL ${CMAKE_BINARY_DIR}/bin/DeployRelease-${ARCH}
)


# Eigen 3

target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_SOURCE_DIR}/modules/eigen)

# OpenAL soft

add_subdirectory(${CMAKE_SOURCE_DIR}/modules/openal-soft OpenAL)

target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_SOURCE_DIR}/modules/openal-soft/include)

set_property(DIRECTORY ${CMAKE_SOURCE_DIR}/modules/openal-soft PROPERTY FOLDER "OpenAL")

target_link_libraries(${PROJECT_NAME} OpenAL)



if(ASCIIR_LOG_VIEWER)
add_subdirectory(${PROJECT_SOURCE_DIR}/ARLogViewer ARLogViewer)
endif()

if(ASCIIR_EXAMPLES)
    foreach(EXAMPLE IN ITEMS ${EXAMPLES})
        add_subdirectory(${PROJECT_SOURCE_DIR}/Examples/${EXAMPLE} ${EXAMPLE})
    endforeach()
    
    set_target_properties(${EXAMPLES} PROPERTIES
        FOLDER "Examples"
    )
endif()
