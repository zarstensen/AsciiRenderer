#pragma once

#ifdef AR_WIN
#define AR_DEBUG_BREAK __debugbreak()
#else
#define AR_DEBUG_BREAK
#endif

#define AR_JOIN_NAME_HELPER(a, b) a##b
#define AR_JOIN_NAME(a, b) AR_JOIN_NAME_HELPER(a, b)

// verify functions for any windows realted functions
#ifdef AR_WIN
	
	// verify the passed windows function
	// if this fails, a error message if logged, and a debug breakpoint is hit
	// if debug is off, the function is still called
	#ifdef AR_DEBUG
	#define AR_WIN_VERIFY(x) {auto AR_JOIN_NAME(ret_val, __LINE__) = x; \
		if(!AR_JOIN_NAME(ret_val, __LINE__)) { \
			char err_msg[256]; \
			FormatMessageA(FORMAT_MESSAGE_FROM_SYSTEM, NULL, GetLastError(), MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), err_msg, 255, NULL); \
			AR_ERR(err_msg); \
			AR_DEBUG_BREAK; \
		} \
		AR_JOIN_NAME(ret_val, __LINE__); }
	#else

	#define AR_WIN_VERIFY(x) x

	#endif
	
	// verify the passed windows function
	// if this fails, a error message if logged, and a debug breakpoint is hit
	// if debug is off, the function is NOT called and simply expands to nothing
	#ifdef AR_DEBUG
	#define AR_WIN_ASSERT(x) { auto AR_JOIN_NAME(ret_val, __LINE__) = x; \
		if(!AR_JOIN_NAME(ret_val, __LINE__)) {\
			char err_msg[256];\
			FormatMessageA(FORMAT_MESSAGE_FROM_SYSTEM, NULL, GetLastError(), MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), err_msg, 255, NULL);\
			AR_ERR(err_msg);\
			assert(false);\
		}}
	#else
	#define AR_WIN_ASSERT(x)
	#endif
#endif

/// @brief same as AR_VERIFY, but a custom message is appended to the error message.
#ifdef AR_DEBUG
#define AR_VERIFY_MSG(x, ...) { auto AR_JOIN_NAME(ret_val, __LINE__) = x; \
	if(!AR_JOIN_NAME(ret_val, __LINE__)) { \
		AR_CORE_ERR("line: ", __LINE__, " File: ", __FILE__, '\n', __VA_ARGS__); \
		AR_DEBUG_BREAK; \
	}}
#else
#define AR_VERIFY_MSG(x, ...) {x}
#endif

/// @brief verifies function passed (x)
/// assumes return value should be true in order of success
/// if failed, log that this failed and hit breakpoint
#ifdef AR_DEBUG
#define AR_VERIFY(x) AR_VERIFY_MSG(x, "Verify failed!")
#else
#define AR_VERIFY(x) {x}
#endif

/// @brief same as AR_VERIFY_MSG
/// also assigns var to the value returned by x
#ifdef AR_DEBUG
#define AR_VERIFY_ASGM_MSG(var, x, ...) { auto AR_JOIN_NAME(ret_val, __LINE__) = x; \
	if(!AR_JOIN_NAME(ret_val, __LINE__)) { \
		AR_CORE_ERR("line: ", __LINE__, " File: ", __FILE__, '\n', __VA_ARGS__); \
		AR_DEBUG_BREAK; \
	}} var = AR_JOIN_NAME(ret_val, __LINE__)
#else
#define AR_VERIFY_ASGM_MSG(var, x, ...) var = x
#endif

/// @brief same as AR_VERIFY
/// also assigns var to the value returned by x
#ifdef AR_DEBUG
#define AR_VERIFY_ASGM(var, x) AR_VERIFY_ASGM_MSG(var, x, "Verify failed!")
#else
#define AR_VERIFY_ASGM(var, x) var = x
#endif

/// @brief if in debug mode, same as AR_ASSERT_MSG, else expand to nothing, meaning x will never be called
#ifdef AR_DEBUG
#define AR_ASSERT(x) AR_ASSERT_MSG(x, "Assert failed at ")
#else
#define AR_ASSERT(x)
#endif

/// @brief same as AR_ASSERT, only on fail, a custom message will be logged to the core log file
#ifdef AR_DEBUG
#define AR_ASSERT_MSG(x, ...) if(!(x)) { \
	AR_CORE_ERR(__VA_ARGS__, " line: ", __LINE__, " File: ", __FILE__); \
	AR_DEBUG_BREAK; \
	assert((false && "Error message can be found in a log file"));}
#else
#define AR_ASSERT_MSG(x, ...)
#endif

/// @brief same as AR_ASSERT, but works in both debug and release mode.
#define AR_ASSERT_VOLATILE(x, ...) if(!(x)) { \
	AR_CORE_ERR(__VA_ARGS__, " line: ", __LINE__, " File: ", __FILE__); \
	AR_DEBUG_BREAK; \
	assert((false && "Error message can be found in a log file"));}