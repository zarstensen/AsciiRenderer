#pragma once

#ifdef AR_WIN

// Define this function in your program and return a heap allocated class derived from Asciir::Terminal
extern Asciir::Terminal* Asciir::CreateTerminal(std::vector<std::string> args);

int main(int argc, char** argv)
{
	std::vector<std::string> args(argc);
	for (size_t i = 0; i < argc; i++)
	{
		args[i] = std::move(argv[i]);
	}

	Asciir::Terminal* terminal = Asciir::CreateTerminal(std::move(args));
	terminal->start();
	delete terminal;
}

#endif
