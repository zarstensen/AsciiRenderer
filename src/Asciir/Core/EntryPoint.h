#pragma once

#include <ChrTrc.h>

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
	void asciirInit(ARApp* app, int argc, char** argv)
	{
		// setup logging
		bool append_logs = false;
		bool save_core = true;
		bool save_client = true;

	#ifdef AR_LOG_APPEND
		append_logs = true;
	#endif
	#ifdef AR_CORE_LOG_DELETE
		save_core = false;
	#endif
	#ifdef AR_CLEINT_LOG_DELETE
		save_core = false;
	#endif

		Asciir::Log::init(save_core, save_client, append_logs);

		// do any platform specific initialization here.
	#if defined(AR_WIN)
		Asciir::WindowsInit();
	#elif defined(AR_LINUX)
		Asciir::LinuxInit();
	#elif defined(AR_MAC)
		// Asciir::MacInit();
	#endif

		// pass the rest of the arguments to the user defined entry point
		std::vector<std::string> args(argc);

		for (int i = 0; i < argc; i++)
		{
			args[i] = std::move(argv[i]);
		}

		// setup the passed ARApp as the main asciir app
		Asciir::ARApp::load(app);
		
		{
		CT_MEASURE_N("Renderer Init");
		Asciir::Renderer::init();
		}

		{
		CT_MEASURE_N("App Init");
		Asciir::ARApp::getApplication()->start(args);
		}
		Asciir::ARApp::getApplication()->startMainLoop();

		{
		CT_MEASURE_N("App shutdown");
		delete Asciir::ARApp::getApplication();
		}
	}
}


/// @brief these macros will define the main function in your program and setup asciir with the passed application
/// @note if other functionality is needed, these definitions can be ignored and a custom entry point can be defined
#define AR_DEFAULT_ENTRYPOINT(APP) AR_DEFAULT_ENTRYPOINT_ARGS(APP,)

#define AR_DEFAULT_ENTRYPOINT_ARGS(APP, ...)				\
int main(int argc, char** argv)								\
{															\
	Asciir::asciirInit(new APP(__VA_ARGS__), argc, argv);\
	return 0;												\
}
