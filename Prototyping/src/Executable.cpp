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

int main()
{
	Exec* exec = new Exec;
	exec->start();
}
