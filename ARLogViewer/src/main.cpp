#include <Asciir.h>
#include "LogViewer.h"

class LogTerm : public Asciir::AREngine
{
public:

	Asciir::LogViewer logviewer;
	std::string m_log_dir;

	LogTerm(const std::string& log_dir)
		:logviewer(log_dir, {
		Asciir::WHITE8,		// Info color White (Default terminal color)
		Asciir::IGREEN8,	// Notify color Green
		Asciir::YELLOW8,	// Warning color Yellow
		Asciir::IYELLOW8,	// Critical color Orange
		Asciir::IRED8		// Error color Red
			}),
		m_log_dir(log_dir)
	{}

	~LogTerm()
	{
		logviewer.close();
	}

	void start()
	{
		Asciir::AREngine::getEngine()->getTerminal().getRenderer().setTitle(m_log_dir);
		Asciir::AREngine::getEngine()->getTerminal().getRenderer().update();

		while (true)
		{
			if (std::filesystem::exists(m_log_dir))
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
			{
				logviewer.reset(std::cout);
				std::cout << "\x1b[2J\x1b[H";
			}
		}
	}
};

Asciir::AREngine* Asciir::createEngine(std::vector<std::string> args)
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