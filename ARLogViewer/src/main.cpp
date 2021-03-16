#include <Asciir.h>
#include "LogViewer.h"

class LogTerm : public Asciir::Terminal
{
public:

	Asciir::LogViewer logviewer;

	LogTerm()
		:logviewer("out.log", {
		Asciir::Color(255, 255, 255),	// Info color White
		Asciir::Color(0, 255, 100),		// Important color Green
		Asciir::Color(255, 255, 0),		// Warning color Yellow
		Asciir::Color(255, 0, 50)		// Error color Red
		})
	{
		while (true)
		{
			if (std::filesystem::exists("out.log"))
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

Asciir::Terminal* Asciir::CreateTerminal()
{
	return new LogTerm;
}
