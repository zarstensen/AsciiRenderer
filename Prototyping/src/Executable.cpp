#include <Asciir.h>
#include <iostream>
#include <Windows.h>
#include <wchar.h>
#include <sstream>

class Exec : public Asciir::Terminal
{
public:
	Exec()
	{

	}

	~Exec()
	{

	}
};

#include <chrono>

Asciir::Terminal* Asciir::CreateTerminal()
{
	AsciiAttr terminal;

	Asciir::FileLog log("out.txt");

	std::ios_base::sync_with_stdio(false);
	std::cin.tie(0);
	
	terminal.setTitle("Colors");
	terminal.setColor(Color(255, 155, 0), Color(100, 100, 100));
	terminal.setAttribute(Asciir::BOLD, true);
	terminal.setBoxed(true);

	std::cout << terminal << "Hello";

	terminal.clear();
	terminal.ansiCode(std::cout);
	
	//log.Log(3, "ERROR", __LINE__, __FILE__, "Hey my name is bo and my age is ", 25);

	std::cin.get();

	return new Exec;
}
