#pragma once
#include "arpch.h"
#include "MacroArguments.h"

namespace Asciir
{
	/// @brief type used for terminal position arguments
	typedef short TInt;
	/// @brief type used for numbers with decimal points (floating point numbers)
#ifdef AR_HIGH_PRECISSION_FLOAT
	typedef double Real;
#else
	typedef float Real;
#endif

	/// @brief class for holding a smart pointer to an object.
	/// 
	///	automaticly sets up an shared pointer depending on the passed arguments, so there is no need to worry about the construction of a shared pointer.
	/// @tparam T the type of the reference pointer 
	template<typename T>
	class Ref : public std::shared_ptr<T>
	{
	public:

		/// @brief default constructor, refrences nothing
		Ref() : std::shared_ptr<T>(nullptr) {}

		/// @brief creates a reference to the passed pointer
		/// @param data data to create reference to
		Ref(T* data) : std::shared_ptr<T>(data) {}
		//Ref(const T& data) : std::shared_ptr<T>(std::make_shared<T>(data)) {}
		/// @brief copy constructor
		Ref(const Ref<T>& other) : std::shared_ptr<T>(other) {}
		/// @brief convert shared_ptr to Ref 
		Ref(const std::shared_ptr<T> other) : std::shared_ptr<T>(other) {}

		/// @brief automaticly converts reference of different type to the current data.
		/// @param other data to be referenced as a different type
		template<typename TOther>
		Ref(const Ref<TOther>& other) : std::shared_ptr<T>(std::dynamic_pointer_cast<T>(other)) {}

		/// @brief automaticly creates a shared pointer from the passed value
		/// @param data data to create reference to
		template<typename TOther, std::enable_if_t<std::is_base_of_v<T, TOther>, bool> = false>
		Ref(const TOther& data) : std::shared_ptr<T>(std::make_shared<TOther>(data)) {}

		// enable_if_t doesn't work in the template here for reasons :/
		/// @brief returns a new Ref object refrencing the same object as the current Ref, but with a different underlying type
		/// @tparam TOther the new underlying type for the new Ref object
		template<typename TOther>
		std::enable_if_t<std::is_abstract_v<T>&& std::is_base_of_v<T, TOther>, Ref<TOther>> cast() { return Ref<TOther>(std::dynamic_pointer_cast<TOther>(*this)); }

		/// @brief allocates the specified type and stores it in the refrence.  
		/// 
		/// this function should only be used to initialize a refrence pointing to nothing.
		/// 
		/// @tparam TOther the type to allocate
		/// @param args arguments passed to TOther's constructor
		template<typename TOther = T, typename ... Args, std::enable_if_t<std::is_base_of_v<TOther, T> || std::is_same_v<TOther, T>, bool> = false>
		void allocate(Args ... args)
		{
			*this = new TOther(args...);
		}

		/// @brief creates a new Ref<T> object pointing to a new instance of type T, copied from the original Ref<T>
		Ref<T> copy()
		{
			return Ref<T>(**this);
		}

		using std::shared_ptr<T>::operator=;

		/// @brief compare the value of two references
		bool operator==(const Ref<T>& other) { return this->get() == other->get(); }
	};

	/// @brief refrence type that should be passed, when passing a refrence, to any of the Renderer::submit functions
	/*template<typename T>
	class RenderRef


	/// @brief alias for RenderRef<T>
	template<typename T>
	using RRef = RenderRef<T>;*/

	/// @brief bit shift left function
	/// @param x amount to be shifted
	/// @param a value to be shifted (default: 1)
	constexpr int BIT_SHL(int x, int a = 1)
	{
		return a << x;
	}

	/// @brief checks is bit is set
	/// @param x position of bit
	/// @param bit collection of bits to be checked
	constexpr bool CHECK_BIT(long long x, int bit)
	{
		return (x >> bit) & 1U;
	}

	/// @brief template check to only enable if passed argument is inerited from other passed argument
	/// @tparam TDerived the type that should be derived from TBase
	/// @tparam TBase the type that TDerived should have as a base
	template<typename TDerived, typename TBase>
	using enable_if_base_of = std::enable_if_t<std::is_base_of_v<TBase, std::decay_t<TDerived>>, bool>;
	
	/// @brief checks if the two passed tempaltes have the same underlying template type
	template<typename T, template<typename...> class TClass>
	struct enable_if_same_template : std::false_type {};

	/// @brief checks if the two passed tempaltes have the same underlying template type
	template<typename ... Args, template<typename...> class TClass>
	struct enable_if_same_template<TClass<Args...>, TClass> : std::true_type {};

	/// @brief signal value for an CTRL + C interrupt
	constexpr int SIG_CTRL_C = SIGINT;

	/// @brief signal value for a command line close interrupt (the user has hit the X in the top right / left, in order to close the program)
#ifdef AR_WIN
	constexpr int SIG_CMD_CLOSE = SIGBREAK;
#else
	constexpr int SIG_CMD_CLOSE = SIGHUP;
#endif
}
