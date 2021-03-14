#pragma once

#ifdef AR_WIN

// Define this function in your program and return a heap allocated class derived from Asciir::Terminal
extern Asciir::Terminal* Asciir::CreateTerminal();

int main(int argc, char** argv)
{
	Asciir::Terminal* terminal = Asciir::CreateTerminal();
	terminal->start();
	delete terminal;
}

#endif
