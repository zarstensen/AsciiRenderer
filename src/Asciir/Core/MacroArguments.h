/// @brief AR_DEBUG: should be defined for debug builds, enables asserts, verifies, and all logging functions
#if defined(DEBUG) || defined(_DEBUG) && !defined(AR_DEBUG)
#define AR_DEBUG
#endif

/// @brief AR_SAFE_RELEASE: should make a release build more error friendly, ie. it will add checks certian places to avoid a crash.
#ifdef AR_DEBUG
#define AR_SAFE_RELEASE
#endif

/// @brief AR_CLIENT_LOG_DIR: where the client logs are written
#ifndef AR_CLIENT_LOG_DIR
#define AR_CLIENT_LOG_DIR "./logs/client_log.log"
#endif

/// @brief AR_CLIENT_LOG_DIR: where the core logs are written
#ifndef AR_CORE_LOG_DIR
#define AR_CORE_LOG_DIR "./logs/core_log.log"
#endif

/// @brief AR_CLIENT_VERBOSITY: sets the verbosity level for the core logging functions
#ifndef AR_CORE_VERBOSITY
#define AR_CORE_VERBOSITY 4
#endif

/// @brief AR_CLIENT_VERBOSITY: sets the verbosity level for the core logging functions
#ifndef AR_CLIENT_VERBOSITY
#define AR_CLIENT_VERBOSITY 4
#endif

/// @brief AR_RENDER_QUEUE_MARGIN: specifies the maximum diffrence between the capacity and the size of the render queue can be before it is reallocated to the last size.
#ifndef AR_RENDER_QUEUE_MARGIN
#define AR_RENDER_QUEUE_MARGIN 10
#endif

/// @brief AR_RENDER_QUEUE_MIN: specifies the minimum capacity of the render queue, -1 = None
#ifndef AR_RENDER_QUEUE_MIN
#define AR_RENDER_QUEUE_MIN 10
#endif

/// @brief AR_RENDERER_QUEUE_MAX: specifies the maximum capacity of the render queue, -1 = None
#ifndef AR_RENDER_QUEUE_MAX
#define AR_RENDER_QUEUE_MAX -1
#endif
