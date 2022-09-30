#pragma once

#include "pch/arpch.h"

#include "Asciir/Platform/PlatformMacros.h"

// Core
#include "Asciir/Core/Core.h"
#include "Asciir/Core/Application.h"
#include "Asciir/Core/Timing.h"
#include "Asciir/Core/Layer.h"
// Core

#include "Asciir/Logging/Log.h"
#include "Asciir/Logging/FileLog.h"

#include "Asciir/Input/Input.h"
#include "Asciir/Input/EventListener.h"
#include "Asciir/Input/KeyCodes.h"

#include "Asciir/Audio/Audio.h"

#include "Asciir/Event/TerminalEventHandler.h"
#include "Asciir/Event/KeyEvent.h"
#include "Asciir/Event/MouseEvent.h"
#include "Asciir/Event/TerminalEvent.h"

#include "Asciir/Entities/ECS.h"
#include "Asciir/Entities/Components.h"
#include "Asciir/Entities/Systems.h"

#include "Asciir/Rendering/TerminalRenderer.h"
#include "Asciir/Rendering/AsciiAttributes.h"

#include "Asciir/Maths/Maths.h"
#include "Asciir/Maths/Vertices.h"
#include "Asciir/Maths/Matrix.h"
#include "Asciir/Maths/Tensor.h"
#include "Asciir/Maths/Lines.h"

#include "Asciir/Rendering/RenderConsts.h"
#include "Asciir/Rendering/Renderer.h"
#include "Asciir/Rendering/Mesh.h"
#include "Asciir/Rendering/Primitives.h"
#include "Asciir/Rendering/Texture.h"

#include "Asciir/Core/AsciirLiterals.h"

// Platform specific headers

#ifdef AR_WIN
#include "Asciir/Platform/Windows/WindowsARAttributes.h"
#include "Asciir/Platform/Windows/WinInit.h"
#elif defined(AR_UNIX)
#include "Asciir/Platform/Unix/UnixARAttributes.h"
#endif

// tools
#include "Asciir/Tools/ProfileLayer.h"
// tools

// Entry point
#include "Asciir/Core/EntryPoint.h"
// Entry Point
