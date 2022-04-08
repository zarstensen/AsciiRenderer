cmake_minimum_required(VERSION 3.9.0)
Project(Asciir)

set(CMAKE_CXX_STANDARD 17)

include(CheckIPOSupported)

# vcpkg
if(CMAKE_TOOLCHAIN_FILE)
    include(${CMAKE_TOOLCHAIN_FILE})
elseif(DEFINED ENV{VCPKG_ROOT} AND NOT ASCIIR_NO_VCPKG)
	message(STATUS "VCPKG_ROOT is defined, including vcpkg.cmake. Define ASCIIR_NO_VCPKG to disable this behaviour")
	include("$ENV{VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake")
endif()

# place all predefined target projects into a filter
set_property(GLOBAL PROPERTY USE_FOLDERS ON)
set(PREDEFINED_TARGETS_FOLDER "CustomTargets")

check_ipo_supported(RESULT ipo_supported OUTPUT error)

if(${MSVC})
    set(CMAKE_CONFIGURATION_TYPES "Debug;InlineDebug;Release;MinSizeRel;RelWithDebInfo;DeployRel" CACHE STRING "" FORCE)

    if(NOT CMAKE_CXX_FLAGS_INLINEDEBUG)
        set(CMAKE_CXX_FLAGS_INLINEDEBUG                 "${CMAKE_CXX_FLAGS_DEBUG} /Ob2" CACHE STRING "" FORCE)
    endif()

    if(NOT CMAKE_EXE_FLAGS_INLINEDEBUG)
        set(CMAKE_EXE_FLAGS_INLINEDEBUG                 "${CMAKE_EXE_FLAGS_DEBUG}" CACHE STRING "" FORCE)
    endif()

    if(NOT CMAKE_LINKER_FLAGS_INLINEDEBUG)
        set(CMAKE_LINKER_FLAGS_INLINEDEBUG              "${CMAKE_LINKER_FLAGS_DEBUG}" CACHE STRING "" FORCE)
    endif()

    if(NOT CMAKE_SHARED_LINKER_FLAGS_INLINEDEBUG)
        set(CMAKE_SHARED_LINKER_FLAGS_INLINEDEBUG       "${CMAKE_SHARED_LINKER_FLAGS_DEBUG}" CACHE STRING "" FORCE)
    endif()

    if(NOT CMAKE_EXE_LINKER_FLAGS_INLINEDEBUG)
        set(CMAKE_EXE_LINKER_FLAGS_INLINEDEBUG          "${CMAKE_EXE_LINKER_FLAGS_DEBUG}" CACHE STRING "" FORCE)
    endif()

    if(NOT CMAKE_CXX_FLAGS_DEPLOYREL)
        set(CMAKE_CXX_FLAGS_DEPLOYREL                   "${CMAKE_CXX_FLAGS_RELEASE}" CACHE STRING "" FORCE)
    endif()

    if(NOT CMAKE_EXE_FLAGS_DEPLOYREL)
        set(CMAKE_EXE_FLAGS_DEPLOYREL                   "${CMAKE_EXE_FLAGS_RELEASE}" CACHE STRING "" FORCE)
    endif()

    if(NOT CMAKE_LINKER_FLAGS_DEPLOYREL)
        set(CMAKE_LINKER_FLAGS_DEPLOYREL                "${CMAKE_LINKER_FLAGS_RELEASE}" CACHE STRING "" FORCE)
    endif()

    if(NOT CMAKE_SHARED_LINKER_FLAGS_DEPLOYREL)
        set(CMAKE_SHARED_LINKER_FLAGS_DEPLOYREL         "${CMAKE_SHARED_LINKER_FLAGS_RELEASE}" CACHE STRING "" FORCE)
    endif()

    if(NOT CMAKE_EXE_LINKER_FLAGS_DEPLOYREL)
        set(CMAKE_EXE_LINKER_FLAGS_DEPLOYREL            "${CMAKE_EXE_LINKER_FLAGS_RELEASE}" CACHE STRING "" FORCE)
    endif()
endif()

# the /ZI option is incompatible with whole program optimization, and as this is not modifyable directly by cmake,
# as well as it being on by default, whole program optimization will need to be enabled manually,
# whilst overriding the /ZI option with /Zi.
#
# this does create a bunch of warnings because of overriding the /ZI option.
if(ipo_supported AND ${MSVC})
add_compile_options("$<$<CONFIG:DEPLOYREL,RELEASE,RELWITHDEBINFO>:/Zi /GL>")
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

# organize folders and files into general variables
set(EXAMPLES
    BasicInputExample
    MouseInputExample
    RendererExample
    TriangleExample
    TextureExample
    SuperAsciirBros
)

set(INCLUDE_DIRS
    src/pch
    src
)

set(SRC_DIR_CORE  
    src/Asciir/Core/Application.cpp
    src/Asciir/Core/Layer.cpp 
    src/Asciir/Core/LayerStack.cpp 
    src/Asciir/Core/Terminal.cpp 
    src/Asciir/Core/Timing.cpp  
    src/Asciir/Core/AsciirLiterals.cpp
)

set(HEADER_DIR_CORE 
    src/Asciir/Core/Core.h 
    src/Asciir/Core/Application.h 
    src/Asciir/Core/EntryPoint.h 
    src/Asciir/Core/Layer.h 
    src/Asciir/Core/LayerStack.h 
    src/Asciir/Core/MacroArguments.h
    src/Asciir/Core/Terminal.h 
    src/Asciir/Core/Timing.h
    src/Asciir/Core/AsciirLiterals.h
)

set(HEADER_DIR_ENTITIES
    src/Asciir/Entities/ECS.h
)

set(SRC_DIR_ENTITIES
    src/Asciir/Entities/ECS.cpp
    src/Asciir/Entities/ECS.ipp
)

set(DIR_EVENT
    src/Asciir/Event/Event.h
    src/Asciir/Event/KeyEvent.h
    src/Asciir/Event/MouseEvent.h
    src/Asciir/Event/TerminalEvent.h
)


set(SRC_DIR_INPUT
	src/Asciir/Input/Input.cpp
)

set(HEADER_DIR_INPUT
    src/Asciir/Input/Input.h
    src/Asciir/Input/KeyCodes.h
    src/Asciir/Input/EventListener.h
)

set(SRC_DIR_AUDIO
    src/Asciir/Audio/Audio.cpp
)

set(HEADER_DIR_AUDIO
    src/Asciir/Audio/Audio.h
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

set(SRC_DIR_MATHS
    src/Asciir/Maths/Maths.cpp
    src/Asciir/Maths/Lines.cpp
    src/Asciir/Maths/Lines.ipp
    src/Asciir/Maths/Vertices.ipp
    src/Asciir/Maths/Vector.ipp
    src/Asciir/Maths/Matrix.ipp
    src/Asciir/Maths/Tensor.ipp
)

set(HEADER_DIR_MATHS
    src/Asciir/Maths/Maths.h
    src/Asciir/Maths/Lines.h
    src/Asciir/Maths/Vertices.h
    src/Asciir/Maths/Vector.h
    src/Asciir/Maths/Matrix.h
    src/Asciir/Maths/Tensor.h
)

set(SRC_DIR_RENDERING
    src/Asciir/Rendering/AsciiAttributes.cpp 
    src/Asciir/Rendering/Mesh.cpp
    src/Asciir/Rendering/Primitives.cpp
    src/Asciir/Rendering/TerminalRenderer.cpp
    src/Asciir/Rendering/Renderer.cpp
    src/Asciir/Rendering/Renderer.ipp
    src/Asciir/Rendering/Texture.cpp
    src/Asciir/Rendering/Texture.ipp
)

set(HEADER_DIR_RENDERING
    src/Asciir/Rendering/AsciiAttributes.h
    src/Asciir/Rendering/Mesh.h
    src/Asciir/Rendering/Primitives.h
    src/Asciir/Rendering/TerminalRenderer.h
    src/Asciir/Rendering/Renderer.h
    src/Asciir/Rendering/RenderConsts.h
    src/Asciir/Rendering/Shader.h
    src/Asciir/Rendering/Texture.h
)

set(SRC_DIR_TOOLS
    src/Asciir/Tools/ProfileLayer.cpp
)

set(HEADER_DIR_TOOLS
    src/Asciir/Tools/ProfileLayer.h
)

# platform specific files
if(WIN32)

    set(SRC_DIR_PLATFORM
        # core
        src/Asciir/Platform/Windows/WinInit.cpp

        # rendering
        src/Asciir/Platform/Windows/WindowsARAttributes.cpp
        src/Asciir/Platform/Windows/WinTermRenderer.cpp

        # input
        src/Asciir/Platform/Windows/WinEventListener.cpp
    )

    set(HEADER_DIR_PLATFORM
        # core
        src/Asciir/Platform/Windows/KeyCodeMap.h
        src/Asciir/Platform/Windows/WinInit.h
        
        #rendering
        src/Asciir/Platform/Windows/WindowsARAttributes.h
        src/Asciir/Platform/Windows/WinTermRenderer.h

        # input
        src/Asciir/Platform/Windows/WinEventListener.h
    )

elseif(UNIX)

    set(SRC_DIR_PLATFORM

        # rendering
        src/Asciir/Platform/Unix/UnixARAttributes.cpp
    )

    set(HEADER_DIR_PLATFORM
        # core
        src/Asciir/Platform/Unix/KeyCodeMap.h

        #rendering
        src/Asciir/Platform/Unix/UnixARAttributes.h
    )

else()
    message(SEND_ERROR "Platform not supported")
endif()

# add the main Asciir library
add_library(${PROJECT_NAME} STATIC
    src/Asciir.h
    src/AsciirHeaders.h
    src/pch/arpch.h
    src/pch/arpch.cpp

    src/Asciir/Platform/PlatformMacros.h

    ${HEADER_DIR_CORE}
    ${SRC_DIR_CORE}

    ${HEADER_DIR_ENTITIES}
    ${SRC_DIR_ENTITIES}

    ${DIR_EVENT}

    ${HEADER_DIR_INPUT}
    ${SRC_DIR_INPUT}

    ${HEADER_DIR_AUDIO}
    ${SRC_DIR_AUDIO}

    ${HEADER_DIR_LOGGING}
    ${SRC_DIR_LOGGING}

    ${HEADER_DIR_MATHS}
    ${SRC_DIR_MATHS}

    ${HEADER_DIR_RENDERING}
    ${SRC_DIR_RENDERING}

    ${SRC_DIR_PLATFORM}
    ${HEADER_DIR_PLATFORM}

    ${SRC_DIR_TOOLS}
    ${HEADER_DIR_TOOLS}
)

if(ipo_supported AND NOT ${MSVC})
set_property(TARGET ${PROJECT_NAME} PROPERTY INTERPROCEDURAL_OPTIMIZATION_RELEASE TRUE)
set_property(TARGET ${PROJECT_NAME} PROPERTY INTERPROCEDURAL_OPTIMIZATION_RELWITHDEBINFO TRUE)
set_property(TARGET ${PROJECT_NAME} PROPERTY INTERPROCEDURAL_OPTIMIZATION_DEPLOYREL TRUE)
endif()

# compile options
if(MSVC)

    set_target_properties(${PROJECT_NAME} PROPERTIES test "$<$<CONFIG:INLINEDEB>:DEBUG> $<$<CONFIG:DEPLOYREL>:RELEASE>")

    # set warning level

    target_compile_options(${PROJECT_NAME} PRIVATE /W4 /external:anglebrackets /external:W0 $<$<CONFIG:DEPLOYREL>:/WX>)
else()
    target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra -Wpedantic -Werror)
endif()

# organize the source files in the generated project
source_group("core/header"          FILES ${HEADER_DIR_CORE})
source_group("core/src"             FILES ${SRC_DIR_CORE})

source_group("entities/header"      FILES ${HEADER_DIR_ENTITIES})
source_group("entities/src"         FILES ${SRC_DIR_ENTITIES})

source_group("event"                FILES ${DIR_EVENT})

source_group("input/header"		    FILES ${HEADER_DIR_INPUT})
source_group("input/src"            FILES ${SRC_DIR_INPUT})

source_group("audio/header"         FILES ${HEADER_DIR_AUDIO})
source_group("audio/src"            FILES ${SRC_DIR_AUDIO})

source_group("logging/header"       FILES ${HEADER_DIR_LOGGING})
source_group("logging/src"          FILES ${SRC_DIR_LOGGING})

source_group("maths/header"         FILES ${HEADER_DIR_MATHS})
source_group("maths/src"            FILES ${SRC_DIR_MATHS})

source_group("rendering/header"     FILES ${HEADER_DIR_RENDERING})
source_group("rendering/src"        FILES ${SRC_DIR_RENDERING})

source_group("tools/header"         FILES ${HEADER_DIR_TOOLS})
source_group("tools/src"            FILES ${SRC_DIR_TOOLS})

source_group("platform/"		    FILES src/Asciir/Platform/PlatformMacros.h)

if(WIN32)
    source_group("platform/windows/header"  FILES ${HEADER_DIR_PLATFORM})
    source_group("platform/windows/src"  	FILES ${SRC_DIR_PLATFORM})
endif()

target_include_directories(${PROJECT_NAME} PUBLIC ${INCLUDE_DIRS})

target_precompile_headers(${PROJECT_NAME} PRIVATE "src/pch/arpch.h")

# set compile definitions depending on configuration
target_compile_definitions(${PROJECT_NAME} PUBLIC
$<$<CONFIG:DEBUG,INLINEDEBUG>:AR_DEBUG>
$<$<CONFIG:RELEASE>:AR_SAFE_RELEASE AR_CORE_VERBOSITY=2 AR_CLIENT_VERBOSITY=4>
$<$<CONFIG:RELEASE,DEBUG,INLINEDEBUG>:AR_CORE_LOG_DIR="../../logs/core_log.log" AR_CLIENT_LOG_DIR="../../logs/client_log.log">
$<$<CONFIG:DEPLOYREL>:AR_CORE_LOG_DIR="./logs/core_log.log" AR_CLIENT_LOG_DIR="./logs/client_log.log">
$<$<BOOL:${ASCIIR_HIGH_PRECISSION_FLOAT}>:AR_HIGH_PRECISSION_FLOAT>)

# packages
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

target_include_directories(${PROJECT_NAME} SYSTEM PUBLIC ${CMAKE_SOURCE_DIR}/modules/eigen)

# SFML

set(BUILD_SHARED_LIBS FALSE CACHE BOOL "ASCIIR specific default value" FORCE)

set(SFML_BUILD_GRAPHICS FALSE CACHE BOOL "ASCIIR specific default value" FORCE)
set(SFML_BUILD_WINDOW FALSE CACHE BOOL "ASCIIR specific default value" FORCE)
set(SFML_BUILD_NETWORK FALSE CACHE BOOL "ASCIIR specific default value" FORCE)

add_subdirectory(${CMAKE_SOURCE_DIR}/modules/SFML SFML)

target_include_directories(${PROJECT_NAME} SYSTEM PUBLIC ${CMAKE_SOURCE_DIR}/modules/SFML/include)

set_property(TARGET sfml-audio PROPERTY FOLDER "modules/SFML")
set_property(TARGET sfml-main PROPERTY FOLDER "modules/SFML")
set_property(TARGET sfml-system PROPERTY FOLDER "modules/SFML")

target_link_libraries(${PROJECT_NAME} SFML::Audio)

# CImg

target_include_directories(${PROJECT_NAME} PUBLIC "${CMAKE_SOURCE_DIR}/modules/CImg")

# enable png and jpeg support
target_compile_definitions(${PROJECT_NAME} PUBLIC cimg_use_png cimg_use_jpeg cimg_verbosity=0)

# libpng, zlib and libjpeg

find_package(JPEG REQUIRED)
target_include_directories(${PROJECT_NAME} PUBLIC  ${JPEG_INCLUDE_DIR})
target_link_libraries(${PROJECT_NAME} ${JPEG_LIBRARIES})

message(STATUS ${JPEG_LIBRARIES})

find_package(PNG REQUIRED)
target_include_directories(${PROJECT_NAME} PUBLIC  ${PNG_INCLUDE_DIRS})
target_link_libraries(${PROJECT_NAME} ${PNG_LIBRARIES})

# EThread

add_subdirectory(${CMAKE_SOURCE_DIR}/modules/EThread EThread)

target_include_directories(${PROJECT_NAME} SYSTEM PUBLIC ${CMAKE_SOURCE_DIR}/modules/ETHread/include)

set_property(TARGET EThread PROPERTY FOLDER "modules/EThread")

target_link_libraries(${PROJECT_NAME} EThread)

# ChrTrcProfiler

add_subdirectory(${CMAKE_SOURCE_DIR}/modules/ChrTrcProfiler ChrTrcProfiler)

target_include_directories(${PROJECT_NAME} SYSTEM PUBLIC ${CMAKE_SOURCE_DIR}/modules/ChrTrcProfiler/include)

set_property(TARGET ChrTrcProfiler PROPERTY FOLDER "modules/ChrTrcProfiler")

target_link_libraries(${PROJECT_NAME} ChrTrcProfiler)

if(ASCIIR_LOG_VIEWER)
    add_subdirectory(${PROJECT_SOURCE_DIR}/ARLogViewer ARLogViewer)
endif()

# add all the examples programmatically
if(ASCIIR_EXAMPLES)
    foreach(EXAMPLE IN ITEMS ${EXAMPLES})
        add_subdirectory(${PROJECT_SOURCE_DIR}/Examples/${EXAMPLE} ${EXAMPLE})
    endforeach()
    
    set_target_properties(${EXAMPLES} PROPERTIES
        FOLDER "Examples"
    )
endif()
