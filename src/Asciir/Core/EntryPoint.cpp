#include "EntryPoint.h"

#include <ChrTrc.h>

void Asciir::asciirInit(ARApp* app, int argc, char** argv)
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

	AR_CORE_INFO("Initialized logger");

	// do any platform specific initialization here.
#if defined(AR_WIN)
	Asciir::WindowsInit();
#elif defined(AR_LINUX)
	Asciir::LinuxInit();
#elif defined(AR_MAC)
	Asciir::MacInit();
#endif

	AR_CORE_INFO("Calling platform init function");

	// pass the rest of the arguments to the user defined entry point
	std::vector<std::string> args(argc);

	for (int i = 0; i < argc; i++)
	{
		args[i] = std::move(argv[i]);
	}

	// setup the passed ARApp as the main asciir app
	Asciir::ARApp::load(app);

	AR_CORE_INFO("Loaded application");

	{
		CT_MEASURE_N("Renderer Init");
		Asciir::Renderer::init();
	}

	AR_CORE_INFO("Initialized renderer");

	{
		CT_MEASURE_N("App Init");
		Asciir::ARApp::getApplication()->start(args);
	}

	AR_CORE_INFO("Started application");

	AR_CORE_INFO("Starting main loop");

	Asciir::ARApp::getApplication()->startMainLoop();

	{
		CT_MEASURE_N("App shutdown");
		delete Asciir::ARApp::getApplication();
	}
}
