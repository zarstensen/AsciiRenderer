#include "arpch.h"
#include "Asciir/Rendering/AsciiAttributes.h"

#ifdef AR_LINUX
namespace Asciir
{
	AsciiAttr::AsciiAttr()
	{
		m_window = initsrc();
		clearColor();
	}

	AsciiAttr::~AsciiAttr()
	{
		endwin();
	}

	std::string AsciiAttr::ansiCode()
	{
		size_t size = 3 + (5 + 4 * 3) * 2;

		for (size_t i = 1; i < 5; i++)
		{
			size += 2ULL * attributes[i];
		}

		std::string escseq;
		escseq.reserve(size);

		ansiCode(escseq);

		return escseq;
	}

	void AsciiAttr::ansiCode(std::string& dst)
	{
		// cursor
		moveCode(dst);

		// formatting
		dst += AR_ANSIS_CSI;
		dst += "0";

		if(attributes[BOLD])
			dst += ";1";

		if (attributes[ITALIC])
			dst += ";3";

		if (attributes[UNDERLINE])
			dst += ";4";

		if (attributes[BLINK])
			dst += ";5";

		if (attributes[STRIKE])
			dst += ";9";

		// foreground color

		dst += ";38;2;";
		dst += std::to_string(m_foreground.red);
		dst += ";";
		dst += std::to_string(m_foreground.green);
		dst += ";";
		dst += std::to_string(m_foreground.blue);
		dst += ";";
		

		// background color
		dst += "48;2;";
		dst += std::to_string(m_background.red);
		dst += ";";
		dst += std::to_string(m_background.green);
		dst += ";";
		dst += std::to_string(m_background.blue);
		dst += 'm';
	}

	void AsciiAttr::moveCode(std::string& dst)
	{
		if (m_should_move)
		{
			dst += AR_ANSIS_CSI;
			dst += std::to_string(m_pos.y + 1) + ';' + std::to_string(m_pos.x + 1) + 'H';

			m_should_move = false;
		}
	}

	Coord AsciiAttr::terminalPos() const
	{
		Coord pos;

		getxy(m_window, pos.x, pos.y);

		return pos;
	}

	TermVert AsciiAttr::terminalSize() const
	{
		return {COLS, LINES};
	}

	TermVert AsciiAttr::maxTerminalSize() const
	{
		// no limits for maximum terminal size on unix systems
		return { std::numeric_limits<TInt>::max, std::numeric_limits<TInt>::max };
	}
}
#endif
