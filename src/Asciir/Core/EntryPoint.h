#pragma once

#ifdef AR_WIN

// Define this function in your program and return a heap allocated class derived from Asciir::Terminal
extern Asciir::Terminal* Asciir::CreateTerminal(std::vector<std::string> args);

int main(int argc, char** argv)
{
	std::filesystem::create_directories(Asciir::CORE_LOG_DIR.parent_path());
	std::filesystem::create_directories(Asciir::CLIENT_LOG_DIR.parent_path());

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

	Asciir::Log::Init(save_core, save_client, append_logs);

	std::vector<std::string> args(argc);
	for (int i = 0; i < argc; i++)
	{
		args[i] = std::move(argv[i]);
	}

	Asciir::Terminal* terminal = Asciir::CreateTerminal(std::move(args));
	terminal->start();
	delete terminal;
}

#endif
