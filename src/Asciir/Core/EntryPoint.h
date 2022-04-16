#pragma once

#include <Asciir.h>

namespace Asciir
{

	/// @brief sets up the asciir environment from the passed arguments
	/// 
	/// acts as the entry point for the Asciir library. The thread the function is called in will also contain the main application loop.
	/// This also means the function is blocking and only returns once the application has been shutdown.
	/// 
	/// @param app the user defined class that implements the application
	/// @param argc should be the same as the main function
	/// @param argv should be the same as the main function
	void asciirInit(ARApp* app, int argc, char** argv);
}


/// @brief these macros will define the main function in your program and setup asciir with the passed application
/// @note if other functionality is needed, these definitions can be ignored and a custom entry point can be defined @see asciirInit()
#define AR_DEFAULT_ENTRYPOINT(APP) AR_DEFAULT_ENTRYPOINT_ARGS(APP,)

#define AR_DEFAULT_ENTRYPOINT_ARGS(APP, ...)				\
int main(int argc, char** argv)								\
{															\
	Asciir::asciirInit(new APP(__VA_ARGS__), argc, argv);\
	return 0;												\
}
