#include <Asciir.h>
#include "LogViewer.h"

class LogTerm : public Asciir::Terminal
{
public:

	Asciir::LogViewer logviewer;

	LogTerm(const std::string& log_dir)
		:logviewer(log_dir, {
		Asciir::Color(204, 204, 204),// Info color White (Default terminal color)
		Asciir::Color(0, 217, 100),		// Important color Green
		Asciir::Color(217, 217, 0),		// Warning color Yellow
		Asciir::Color(217, 50, 50)		// Error color Red
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
	std::string log_dir = "out.log";
	if (args.size() > 2)
	{
		std::cout << "Cannot take more than one commandline argument!\n";
		throw std::runtime_error("Cannot take more than one commandline argument!\n");
	} 
	else if (args.size() == 2)
	{
		log_dir = args[1];
	}

	return new LogTerm(log_dir);
}
