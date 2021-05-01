cmake_minimum_required(VERSION 3.0.0)
Project(Asciir)

set(CMAKE_CXX_STANDARD 20)

# place all predefined target projects into a filter
set_property(GLOBAL PROPERTY USE_FOLDERS ON)
set(PREDEFINED_TARGETS_FOLDER "CustomTargets")

set(CMAKE_CONFIGURATION_TYPES "Debug;Release" CACHE STRING "" FORCE)

# options
option(LOG_VIEWER "builds an executable to view log files" ON)
option(PROTOTYPING "build example project" OFF)

if(CMAKE_SIZEOF_VOID_P EQUAL 8)
set(ARCH "x64")
elseif(CMAKE_SIZEOF_VOID_P EQUAL 4)
set(ARCH "x86")
else()
message(STATUS "unknown architecture")
endif()

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
)

set(HEADER_DIR_CORE 
    src/Asciir/Core/Core.h 
    src/Asciir/Core/Engine.h 
    src/Asciir/Core/EntryPoint.h 
    src/Asciir/Core/Layer.h 
    src/Asciir/Core/LayerStack.h 
    src/Asciir/Core/Terminal.h 
    src/Asciir/Core/Timing.h 
) 

set(SRC_DIR_LOGGING
    src/Asciir/Logging/FileLog.cpp 
    src/Asciir/Logging/FileLog.ipp
    src/Asciir/Logging/ErrLog.ipp  
    src/Asciir/Logging/Log.cpp 
    src/Asciir/Logging/Log.ipp 
)

set(HEADER_DIR_LOGGING
    src/Asciir/Logging/FileLog.h
    src/Asciir/Logging/Log.h
)

set(SRC_DIR_MATH
    src/Asciir/Math/lines.cpp
    src/Asciir/Math/Vertices.ipp
    src/Asciir/Math/Matrix.ipp
)

set(HEADER_DIR_MATH
    src/Asciir/Math/lines.h
    src/Asciir/Math/Vertices.h
    src/Asciir/Math/Matrix.h
)

set(SRC_DIR_RENDERING
    src/Asciir/Rendering/AsciiAttributes.cpp 
    src/Asciir/Rendering/TerminalRender.cpp
)

set(HEADER_DIR_RENDERING
    src/Asciir/Rendering/AsciiAttributes.h
    src/Asciir/Rendering/TerminalRender.h
    src/Asciir/Rendering/RenderConsts.h
)

if(WIN32)
set(SRC_DIR_PLATFORM
    # core
    src/Asciir/Platform/Windows/WindowsTerminal.cpp
    src/Asciir/Platform/Windows/WinInput.cpp
    src/Asciir/Platform/Windows/KeyCodeMap.cpp

    # rendering
    src/Asciir/Platform/Windows/WindowsARAttributes.cpp
    src/Asciir/Platform/Windows/WindowsARAttributes.ipp
)

set(HEADER_DIR_PLATFORM
    # core
    src/Asciir/Platform/Windows/WindowsTerminal.h
    src/Asciir/Platform/Windows/WinInput.h
    src/Asciir/Platform/Windows/KeyCodeMap.h
)
else()
message(SEND_ERROR "Platform not supported")
endif()

add_library(${PROJECT_NAME} STATIC
    src/Asciir.h
    src/pch/arpch.h
    src/pch/arpch.cpp

    ${HEADER_DIR_CORE}
    ${SRC_DIR_CORE}

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

source_group("logging/header"     FILES ${HEADER_DIR_LOGGING})
source_group("logging/src"        FILES ${SRC_DIR_LOGGING})

source_group("math/header"        FILES ${HEADER_DIR_MATH})
source_group("math/src"           FILES ${SRC_DIR_MATH})

source_group("rendering/header"   FILES ${HEADER_DIR_RENDERING})
source_group("rendering/src"      FILES ${SRC_DIR_RENDERING})

if(WIN32)
source_group("platform/windows/header"  FILES ${HEADER_DIR_PLATFORM})
source_group("platform/windows/src"  FILES ${SRC_DIR_PLATFORM})
endif()

target_include_directories(${PROJECT_NAME} PUBLIC ${INCLUDE_DIRS})

target_compile_definitions(${PROJECT_NAME} PUBLIC $<$<CONFIG:DEBUG>:AR_DEBUG> AR_CORE_LOG_DIR="${CMAKE_BINARY_DIR}/logs/core_log.log" AR_CLIENT_LOG_DIR="${CMAKE_BINARY_DIR}/logs/client_log.log")

message(STATUS ${CMAKE_BINARY_DIR})

set_target_properties(${PROJECT_NAME}
    PROPERTIES
    OUTPUT_NAME ${PROJECT_NAME}
    ARCHIVE_OUTPUT_DIRECTORY_DEBUG ${CMAKE_BINARY_DIR}/bin/Debug-${ARCH}
    ARCHIVE_OUTPUT_DIRECTORY_RELEASE ${CMAKE_BINARY_DIR}/bin/Release-${ARCH}
)

# Eigen 3

target_include_directories(${PROJECT_NAME} PUBLIC ${PROJECT_SOURCE_DIR}/modules/eigen)

if(LOG_VIEWER)
add_subdirectory(${PROJECT_SOURCE_DIR}/ARLogViewer ARLogViewer)
endif()

if(PROTOTYPING)
add_subdirectory(${PROJECT_SOURCE_DIR}/Prototyping Prototyping)
endif()
