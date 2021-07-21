#pragma once

#include "Asciir/Platform/PlatformMacros.h"

#include <Eigen/Eigen>

#include <functional>
#include <memory>
#include <algorithm>
#define _USE_MATH_DEFINES
#include <cmath>
#include <thread>
#include <chrono>
#include <filesystem>
#include <type_traits>
#include <limits>

#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <future>
#include <vector>
#include <array>
#include <tuple>
#include <map>
#include <variant>
#include <cassert>
#include <csignal>

#ifdef AR_WIN
#include <Windows.h>
#undef min
#undef max
#else
#include <X11/Xlib.h>
#include <X11/Xutil.h>

// for terminal size
#include <sys/ioctl.h>
#include <unistd.h>

#include <termios.h>

#endif
