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
	Asciir::FileLog logger("out.log");

	logger.Log(0, "INFO", __LINE__, __FILE__, "hej", "med", 25);
	logger.Log(1, "IMPORTANT", __LINE__, __FILE__, "hej", "med", 25);
	logger.Log(2, "WARNING", __LINE__, __FILE__, "hej", "med", 25);
	logger.Log(3, "ERROR", __LINE__, __FILE__, "hej", "med", 25);

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
