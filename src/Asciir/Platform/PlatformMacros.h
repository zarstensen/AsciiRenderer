#pragma once

// this header file is responsible for defining the macros describing the current platform Asciir is being built on.
// thus the user should not need to define these macros themselves and instead let this header do all the work.

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)

/// @brief defined when platform is windows (both 64 bit and 32 bit)
#define AR_WIN

#ifdef _WIN64
/// @brief defined when platform is windows 64 bit
#define AR_WIN_64
#else
/// @brief defined when platform is windows 32 bit
#define AR_WIN_32
#endif

#elif __unix__ or __APPLE__ or __linux__

/// @brief defined if on a unix platform
#define AR_UNIX

#if __APPLE__
#include <TargetConditionals.h>
#if TARGET_IPHONE_SIMULATOR
#error "IPhone simulator is not a supported platform"
#elif TARGET_OS_IPHONE
#error "IPhone OS is not a supported platform"
#elif TARGET_OS_MAC
/// @brief defined if on MacOS
#define AR_MAC
#else
#error "Unknown Apple platform"
#endif
#elif __linux__
/// @brief defined if on Linux
#define AR_LINUX
#elif defined(_POSIX_VERSION)
/// @brief defined if on posix OS
#define AR_POSIX
#else
#error "Unknown compiler"
#endif
#endif
