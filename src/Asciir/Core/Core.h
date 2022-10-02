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

	/// @brief alias for the underlying file path data type
	typedef std::filesystem::path Path;


	// ============ Ref ============

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
		std::enable_if_t<std::is_base_of_v<T, TOther> || std::is_base_of_v<TOther, T>, Ref<TOther>> cast() { return Ref<TOther>(std::dynamic_pointer_cast<TOther>(*this)); }

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

		operator T* ()
		{
			return get();
		}

		/// @brief creates a new Ref<T> object pointing to a new instance of type T, copied from the original Ref<T>
		Ref<T> copy()
		{
			return Ref<T>(**this);
		}

		using std::shared_ptr<T>::operator=;

		/// @brief compare the value of two references
		bool operator==(const Ref<T>& other) const { return this->get() == other.get(); }
	};

	/// @brief refrence type that should be passed, when passing a refrence, to any of the Renderer::submit functions
	/*template<typename T>
	class RenderRef


	/// @brief alias for RenderRef<T>
	template<typename T>
	using RRef = RenderRef<T>;*/

	// ============ utf-8 classes ============

	/// @brief checks if T is a character type.
	template<typename T>
	struct is_char : public std::false_type {};

	template<>
	struct is_char<char> : public std::true_type {};

#if 0
	template<>
	struct is_char<wchar_t> : public std::true_type {};

	template<>
	struct is_char<char16_t> : public std::true_type {};
#endif // TODO: add support for utf-16 in the U8Char class.

	template<>
	struct is_char<char32_t> : public std::true_type {};


	/// @brief clas representing a singular utf-8 encoded character.
	class U8Char
	{
	protected:
		/// @brief the maximum size of a UTF8 character, in bytes
		static constexpr int UTF_CODE_LEN = 4;

		/// @brief 4 byte character storing the U8Char's utf-8 data.
		char32_t m_symbol;

	public:

		U8Char() = default;

		/// @brief initializes the U8Char with the passed utf-32 encoded character, after encoding it to its corresponding utf-8 encoding.
		constexpr U8Char(char32_t utf32_char)
			: U8Char(fromCode(utf32_char)) {}

		/// @brief constructs a U8Char instance from the given utf-8 binary sequence
		/// if a utf-32 character should be converted to a utf-8 character, see U8Char(char32_t)
		constexpr U8Char(char32_t* utf8_char)
			: m_symbol(*utf8_char) {}

		/// @brief constructs a U8Char from the given utf-32 code point
		static constexpr U8Char fromCode(char32_t code_point)
		{
			// the two byte character is simply mapped onto the multi byte UTF-8 represeentation.
			// eg. the binary value 00010100011 simply becomes 110 000101 10 1000011
			U8Char c;
			c.m_symbol = 0;

			// the char32_t should be seen as a 4 byte buffer, instead of a single char.
			char* data = (char*)c;

			// start by checking if code point is ascii
			if (code_point < 128)
			{
				data[0] = (char)code_point;
			}
			else
			{
				for (size_t i = 1; i < 4; i++)
				{
					// find the length of the UTF-8 encoding
					if (code_point >> (i * 6 + 6 - i) == 0)
					{
						for (size_t j = 0; j < i; j++)
							data[i - j] = ((code_point >> (j * 6)) & 0x3F) + 0x80;


						data[0] = (char)(code_point >> (i * 6));

						for (size_t j = 0; j < i + 1; j++)
							data[0] = data[0] + (0x80 >> j);

						break;
					}
				}
			}

			return c;
		}

		/// @brief returns the utf-32 encoded version of the passed U8Char's utf-8 character.
		static constexpr char32_t toCode(U8Char utf8_char)
		{
			char* utf8_buffer = (char*)utf8_char;

			uint8_t l = length(utf8_char);

			if (l == 1)
				return ((char*)utf8_char)[0];
			else
			{
				char32_t code_point = 0;

				// generate the masks for all the utf-8 bytes
				// bytes 2, 3 and 4, all have identical masks, so there is no need to generate unique masks for them.
				// the first bytes mask is dependent on the number of bytes in the utf-8 character.

				// for more info, see https://en.wikipedia.org/wiki/UTF-8#:~:text=.%5B12%5D-,Encoding,-%5Bedit%5D

				// first byte mask:
				// 1 byte:
				//		01111111 (ascii, handled earlier)
				// 2 bytes:
				//		00011111
				// 3 bytes:
				//		00001111
				// 4 bytes:
				//		00000111

				uint8_t first_mask = 0b00111111;

				first_mask >>= l - 1;

				// the rest of the bytes have the following masks:
				//		00111111

				uint8_t rest_mask = 0b00111111;
				uint8_t buff_pos = 0;

				// fill up the cp_buffer whist keeping track of unfilled bits, that should be filled by the next bytes.
				for (uint8_t i = l - 1; i > 0; i--)
				{
					char32_t tmp = utf8_buffer[i];
					code_point |= (tmp & rest_mask) << buff_pos;
					buff_pos += 6;
				}

				char32_t tmp = utf8_buffer[0];
				code_point |= (tmp & first_mask) << buff_pos;

				return code_point;
			}
		}

		/// @brief returns the number of bytes required in order to store the current utf-8 character.
		static constexpr uint8_t length(U8Char c)
		{
			char* u8_str = (char*)c;

			// check if the first bit in the first byte is 0, if so the char size is 1
			if (~(u8_str[0] >> 7) & 1U)
				return 1;
			// check if the first bits in the first bytes are 110 (size 2), 1110 (size 3) or 11110 (size 4)
			else
			{
				uint8_t check = 0b110;
				uint8_t mask = 0b111;

				for (uint8_t i = 2; i <= 4; i++)
				{
					if (!(bool)(((u8_str[0] >> (7 - i)) & mask) ^ check))
						return i;

					check = (check << 1) + (uint8_t)0b10;
					mask = (mask << 1) + (uint8_t)0b1;
				}
			}

			// first bit was not formatted correctly
			return 0;
		}

		/// @brief convert the utf-8 encoded character from a char32_t pointer to a TChar pointer
		/// should be used for printing, as the char32_t value cannot be printed directly to the terminal, but instead needs to be converted to a c string.
		template<typename TChar, std::enable_if_t<is_char<TChar>::value, bool> = false>
		explicit constexpr operator TChar* ()
		{
			return (TChar*)&m_symbol;
		}

		/// @see operator::TChar*
		template<typename TChar, std::enable_if_t<is_char<TChar>::value, bool> = false>
		explicit constexpr operator const TChar* () const
		{
			return (TChar*)&m_symbol;
		}

		/// @brief retrieves the utf-8's corresponding utf-32 character.
		explicit constexpr operator char32_t() const
		{
			return toCode(*this);
		}

		/// @see operator::char32_t()
		explicit constexpr operator uint32_t() const
		{
			return (char32_t)*this;
		}

		/// @brief checks if two U8Chars contain the same utf-8 character
		constexpr bool operator==(U8Char other) const { return m_symbol == other.m_symbol; }
		constexpr bool operator!=(U8Char other) const { return !(*this == other); }
		/// @brief convert the passed character to its utf-8 representation, and check if it matches the value stored in the current U8Char instance.
		template<typename TChar, std::enable_if_t<is_char<TChar>::value, bool> = false>
		constexpr bool operator==(TChar c) const { return *this == fromCode((char32_t)c); }
		template<typename TChar, std::enable_if_t<is_char<TChar>::value, bool> = false>
		constexpr bool operator!=(TChar c) const { return !(*this == c); }
		/// @brief check if the passed utf-8 encoded byte array matches the currently stored utf-8 encoded character.
		constexpr bool operator==(const char* u8_bytes) const { return m_symbol == *(char32_t*)u8_bytes; }
		constexpr bool operator!=(const char* u8_bytes) const { return !(*this == u8_bytes); }
	};

	/// @brief class representing a string consisting of U8Char's
	/// same as std::basic_string<U8Char>, with added constructors
	class U8String : public std::basic_string<U8Char>
	{
	public:
		using std::basic_string<U8Char>::basic_string;

		/// @brief convert the given char string to its corresponding utf-8 encoded string.
		template<typename TChar>
		U8String(const TChar* str)
			: std::basic_string<U8Char>(std::char_traits<TChar>::length(str), U'\0')
		{
			for (size_t i = 0; i < std::char_traits<TChar>::length(str); i++)
				(*this)[i] = U8Char((char32_t)str[i]);
		}
	};

	inline std::ostream& operator<<(std::ostream& stream, U8Char c)
	{
		stream << (char*)c;
		return stream;
	}

	inline std::ostream& operator<<(std::ostream& stream, const U8String& str)
	{
		for (U8Char c : str)
			stream << c;

		return stream;
	}

	// ============ OTHER ============

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
