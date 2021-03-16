#include <Asciir.h>
#include "LogViewer.h"

class LogTerm : public Asciir::Terminal
{
public:

	Asciir::LogViewer logviewer;

	LogTerm(const std::string& log_dir)
		:logviewer(log_dir, {
		Asciir::Color(204, 204, 204),// Info color White (Default terminal color)
		Asciir::Color(0, 255, 100),		// Important color Green
		Asciir::Color(255, 255, 0),		// Warning color Yellow
		Asciir::Color(255, 0, 50)		// Error color Red
		})
	{
		while (true)
		{
			if (std::filesystem::exists(log_dir))
			{
				logviewer.open();
				break;
			}
		}

		while (true)
		{
			if (logviewer.hasLogs())
				logviewer.logLineOut(std::cout);
		}
	}

	~LogTerm()
	{
		logviewer.close();
	}
};

Asciir::Terminal* Asciir::CreateTerminal(std::vector<std::string> args)
{
	for (std::string elem : args)
	{
		std::cout << elem << '\n';
	}
	return new LogTerm("out.log");
}
