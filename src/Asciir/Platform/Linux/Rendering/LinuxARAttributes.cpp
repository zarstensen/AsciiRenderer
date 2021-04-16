#include "arpch.h"
#include "Asciir/Rendering/AsciiAttributes.h"

#ifdef AR_LINUX
namespace Asciir
{
	std::string AsciiAttr::ansiCode() const
	{
		size_t size = 3 + (5 + 4 * 3) * 2;

		for (size_t i = 0; i < 5; i++)
		{
			size += 2 * attributes[i];
		}

		for (size_t i = 5; i < 8; i++)
		{
			size += 3 * attributes[i];
		}

		std::string escseq;
		escseq.reserve(size);

		ansiCode(escseq);

		return escseq;
	}

	void AsciiAttr::ansiCode(std::string& dst) const
	{
		// formatting
		dst += AR_ANSIS_CSI;
		dst += "0";

		if (attributes[BOLD])
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
}
#endif
