#pragma once

#include "Asciir/Platform/PlatformMacros.h"

#include <functional>
#include <memory>
#include <algorithm>
#define _USE_MATH_DEFINES
#include <cmath>
#include <thread>
#include <chrono>
#include <filesystem>
#include <type_traits>
#include <typeindex>
#include <limits>
#include <thread>

#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <future>
#include <vector>
#include <unordered_set>
#include <unordered_map>
#include <queue>
#include <array>
#include <tuple>
#include <map>
#include <variant>
#include <cassert>
#include <csignal>

#ifdef AR_WIN
#define NOMINMAX
#include <Windows.h>
#else
#include <X11/Xlib.h>
#include <X11/Xutil.h>

// for terminal size
#include <sys/ioctl.h>
#include <unistd.h>

#include <termios.h>
#endif

// Modules
#include <Eigen/Eigen>

#include <ChrTrcProfiler.h>
#include <ChrTrcCompileTimeString.h>

#include <ETH.h>

#include <SFML/Audio.hpp>
// Modules
