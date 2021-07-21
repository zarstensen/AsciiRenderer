#pragma once

#ifdef AR_WIN
#define AR_DEBUG_BREAK __debugbreak()
#else
#define AR_DEBUG_BREAK
#endif

#define AR_JOIN_NAME_HELPER(a, b) a##b
#define AR_JOIN_NAME(a, b) AR_JOIN_NAME_HELPER(a, b)

#ifdef AR_DEBUG
#define AR_WIN_VERIFY(x) auto AR_JOIN_NAME(ret_val, __LINE__) = x; if(!AR_JOIN_NAME(ret_val, __LINE__)) {\
char err_msg[256];\
FormatMessageA(FORMAT_MESSAGE_FROM_SYSTEM, NULL, GetLastError(), MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), err_msg, 255, NULL);\
AR_ERR(err_msg);\
__debugbreak();\
}\
AR_JOIN_NAME(ret_val, __LINE__)

#else

#define AR_WIN_VERIFY(x) x

#endif

#ifdef AR_DEBUG
#define AR_WIN_ASSERT(x) auto AR_JOIN_NAME(ret_val, __LINE__) = x; if(!AR_JOIN_NAME(ret_val, __LINE__)) {\
char err_msg[256];\
FormatMessageA(FORMAT_MESSAGE_FROM_SYSTEM, NULL, GetLastError(), MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), err_msg, 255, NULL);\
AR_ERR(err_msg);\
assert(false);\
}
#else
#define AR_WIN_ASSERT(x)
#endif

#ifdef AR_DEBUG
#define AR_VERIFY(x) auto AR_JOIN_NAME(ret_val, __LINE__) = x; if(!AR_JOIN_NAME(ret_val, __LINE__)) {AR_CORE_ERR("Verify failed at line: ", __LINE__, " File: ", __FILE__);AR_DEBUG_BREAK;}
#else
#define AR_VERIFY(x) x;
#endif

#ifdef AR_DEBUG
#define AR_VERIFY_ASGM(var, x) auto AR_JOIN_NAME(ret_val, __LINE__) = x; if(!AR_JOIN_NAME(ret_val, __LINE__)) {AR_CORE_ERR("Verify failed at line: ", __LINE__, " File: ", __FILE__);AR_DEBUG_BREAK;} var = AR_JOIN_NAME(ret_val, __LINE__)
#else
#define AR_VERIFY_ASGM(var, x) var = x
#endif

#ifdef AR_DEBUG
#define AR_ASSERT(x) AR_ASSERT_MSG(x, "Assert failed at ")
#else
#define AR_ASSERT(x)
#endif

#ifdef AR_DEBUG
#define AR_ASSERT_MSG(x, ...) if(!(x)) {AR_CORE_ERR(__VA_ARGS__, " line: ", __LINE__, " File: ", __FILE__);AR_DEBUG_BREAK;assert((false && "Error message can be found in a log file"));}
#else
#define AR_ASSERT_MSG(x, ...)
#endif

#define AR_ASSERT_VOLATILE(x, ...) if(!(x)) {AR_CORE_ERR(__VA_ARGS__, " line: ", __LINE__, " File: ", __FILE__);AR_DEBUG_BREAK;assert((false && "Error message can be found in a log file"));}