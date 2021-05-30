#pragma once

#include "pch/arpch.h"

#include "Asciir/Platform/PlatformMacros.h"

// Core
#include "Asciir/Core/Core.h"
#include "Asciir/Core/Engine.h"
#include "Asciir/Core/Terminal.h"
#include "Asciir/Core/Timing.h"
#include "Asciir/Core/Layer.h"
// Core

#include "Asciir/Logging/Log.h"
#include "Asciir/Logging/FileLog.h"

#include "Asciir/Input/Input.h"
#include "Asciir/Input/KeyCodes.h"

#include "Asciir/Event/KeyEvent.h"
#include "Asciir/Event/MouseEvent.h"
#include "Asciir/Event/TerminalEvent.h"

#include "Asciir/Rendering/TerminalRenderer.h"
#include "Asciir/Rendering/AsciiAttributes.h"
#include "Asciir/Math/Vertices.h"
#include "Asciir/Math/Matrix.h"
#include "Asciir/Math/Tensor.h"
#include "Asciir/Math/Lines.h"

#include "Asciir/Rendering/RenderConsts.h"

// Platform specific headers

#ifdef AR_WIN
#include "Asciir/Platform/Windows/WindowsARAttributes.h"
#elif defined(AR_UNIX)
#include "Asciir/Platform/Unix/UnixARAttributes.h"
#endif
