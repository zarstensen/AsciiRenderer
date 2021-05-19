#pragma once

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
	#define AR_WIN
	
	#ifdef _WIN64
		#define AR_WIN_64
	#else
		#define AR_WIN_32
	#endif

#elif __unix__ or __APPLE__ or __linux__
	
	#define AR_UNIX
	
	#if __APPLE__
		#include <TargetConditionals.h>
		#if TARGET_IPHONE_SIMULATOR
			#error "IPhone simulator is not a supported platform"
		#elif TARGET_OS_IPHONE
			#error "IPhone OS is not a supported platform"
		#elif TARGET_OS_MAC
			#define AR_MAC
		#else
			#error "Unknown Apple platform"
		#endif
	#elif __linux__
		#define AR_LINUX
	#elif defined(_POSIX_VERSION)
		#define AR_POSIX
	#else
		#error "Unknown compiler"
	#endif
#endif
