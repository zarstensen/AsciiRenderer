#pragma once

#if defined(DEBUG) || defined(_DEBUG) && !defined(AR_DEBUG)
#define AR_DEBUG
#endif

#ifdef AR_DEBUG
#define AR_SAFE_RELEASE
#endif

#ifndef AR_CLIENT_LOG_DIR
#define AR_CLIENT_LOG_DIR "./logs/client_log.log"
#endif

#ifndef AR_CORE_LOG_DIR
#define AR_CORE_LOG_DIR "./logs/core_log.log"
#endif

namespace Asciir
{
	typedef short TInt;
	typedef float Real;

	constexpr int BIT_SHL(int x, int a = 1)
	{
		return a << x;
	}

	constexpr bool CHECK_BIT(long long x, int bit)
	{
		return (x >> bit) & 1U;
	}

	constexpr int SIG_CTRL_C = SIGINT;
	#ifdef AR_WIN
	constexpr int SIG_CMD_CLOSE = SIGBREAK;
	#else
	constexpr int SIG_CMD_CLOSE = SIGHUP;
	#endif
}
