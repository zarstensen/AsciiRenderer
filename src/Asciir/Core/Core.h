#pragma once
#include "arpch.h"
#include "MacroArguments.h"

namespace Asciir
{
	// type used for terminal position arguments
	typedef short TInt;
	// type used for numbers with decimal points (floating point numbers)
	#ifdef AR_HIGH_PRECISSION_FLOAT
	typedef double Real;
	#else
	typedef float Real;
	#endif

	template<typename T>
	class Ref: public std::shared_ptr<T>
	{
	public:
		Ref() : std::shared_ptr<T>(nullptr) {}
		Ref(T* data): std::shared_ptr<T>(data) {}
		Ref(const T& data): std::shared_ptr<T>(std::make_shared<T>(data)) {}
		Ref(const Ref<T>& other): std::shared_ptr<T>(other) {}
		Ref(const std::shared_ptr<T> other) : std::shared_ptr<T>(other) {}
		
		template<typename TOther>
		Ref<TOther> cast() { return Ref<TOther>(dynamic_pointer_cast<TOther>(*this)); };

		using std::shared_ptr<T>::operator=;
	};

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
