#include <Asciir.h>

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

Asciir::Terminal* Asciir::CreateTerminal()
{
	return new Exec;
}
