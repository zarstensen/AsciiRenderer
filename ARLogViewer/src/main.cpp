#include <Asciir.h>
#include "LogViewer.h"

class LogTerm : public Asciir::Terminal
{
public:

	Asciir::LogViewer logviewer;

	LogTerm(const std::string& log_dir)
		:logviewer(log_dir, {
		Asciir::WHITE8,		// Info color White (Default terminal color)
		Asciir::IGREEN8,	// Notify color Green
		Asciir::YELLOW8,	// Warning color Yellow
		Asciir::IYELLOW8,	// Critical color Orange
		Asciir::IRED8		// Error color Red
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
			else if (logviewer.pos() > logviewer.size())
				logviewer.reset(std::cout);
		}
	}

	~LogTerm()
	{
		logviewer.close();
	}
};

Asciir::Terminal* Asciir::CreateTerminal(std::vector<std::string> args)
{
	std::string log_dir = AR_CORE_LOG_DIR;
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