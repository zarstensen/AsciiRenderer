#include <Asciir.h>
#include "LogViewer.h"

class LogTerm : public Asciir::Terminal
{
public:
	LogTerm()
	{

	}

	~LogTerm()
	{

	}
};

Asciir::Terminal* Asciir::CreateTerminal()
{
	Asciir::LogViewer logviewer("out.log", {
		Color(255, 255, 255),	// Info color White
		Color(0, 255, 100),		// Important color Green
		Color(255, 255, 0),		// Warning color Yellow
		Color(255, 0, 50)		// Error color Red
		});

	while (true)
	{
		if (std::filesystem::exists("out.log"))
		{
			logviewer.open();
			break;
		}
	}

	while (logviewer.hasLogs())
	{
		logviewer.logLineOut(std::cout);	
	}

	return new LogTerm;
}
