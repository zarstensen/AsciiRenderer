#pragma once

#include "Asciir/Rendering/RenderConsts.h"

namespace Asciir
{
	template<typename TStream>
	void AsciiAttr::ansiCode(TStream& stream, bool is_newline)
	{

		#if 0
		SetConsoleTextAttribute(m_hConsole, DEFAULT_FOREGROUND
			| COMMON_LVB_GRID_HORIZONTAL * attributes[TOP]
			| COMMON_LVB_GRID_LVERTICAL * attributes[LEFT]
			| COMMON_LVB_GRID_RVERTICAL * attributes[RIGHT]);
		#endif

		// if nothing is change do not modify the stream
		bool has_changed = false;
		for (size_t i = 0; i < ATTR_COUNT; i++)
			if (attributes[i] != last_attributes[i])
			{
				has_changed = true;
				break;
			}


		if (!has_changed && m_foreground != m_last_foreground)
			has_changed = true;

		if (!has_changed && m_background != m_last_background)
			has_changed = true;

		if (m_cleared)
			has_changed = true;
		

		if (!has_changed)
			return;

		// formatting
		stream << AR_ANSIS_CSI;

		if (attributes[ITALIC] && (!last_attributes[ITALIC] || m_cleared))
			stream << ";3";
		else if (!attributes[ITALIC] && (last_attributes[ITALIC] || m_cleared))
			stream << ";23";

		if (attributes[UNDERLINE] && (!last_attributes[UNDERLINE] || m_cleared))
			stream << ";4";
		else if (!attributes[UNDERLINE] && (last_attributes[UNDERLINE] || m_cleared))
			stream << ";24";

		if (attributes[BLINK] && (!last_attributes[BLINK] || m_cleared))
			stream << ";5";
		else if (!attributes[BLINK] && (last_attributes[BLINK] || m_cleared))
			stream << ";25";

		if (attributes[STRIKE] && (!last_attributes[STRIKE] || m_cleared))
			stream << ";9";
		else if (!attributes[STRIKE] && (last_attributes[STRIKE] || m_cleared))
			stream << ";29";

		// foreground color
		if (attributes[BOLD] && ((!last_attributes[BOLD] || m_foreground != m_last_foreground) || m_cleared || is_newline))
		{
			unsigned char red = m_foreground.red + AR_BOLD_DIFF;
			unsigned char green = m_foreground.green + AR_BOLD_DIFF;
			unsigned char blue = m_foreground.blue + AR_BOLD_DIFF;

			red = red > m_foreground.red ? red : 255;
			green = green > m_foreground.green ? green : 255;
			blue = blue > m_foreground.blue ? blue : 255;

			stream << ";38;2;";
			stream << std::to_string(red);
			stream << ";";
			stream << std::to_string(green);
			stream << ";";
			stream << std::to_string(blue);
			stream << ";";
		}
		else if (m_foreground != m_last_foreground || m_cleared || is_newline)
		{
			stream << ";38;2;";
			stream << std::to_string(m_foreground.red);
			stream << ";";
			stream << std::to_string(m_foreground.green);
			stream << ";";
			stream << std::to_string(m_foreground.blue);
			stream << ";";
		}

		// background color
		if (m_background != m_last_background || m_cleared || is_newline)
		{
			stream << "48;2;";
			stream << std::to_string(m_background.red);
			stream << ';';
			stream << std::to_string(m_background.green);
			stream << ';';
			stream << std::to_string(m_background.blue);
		}

		stream << 'm';
		last_attributes = attributes;
		m_last_foreground = m_foreground;
		m_last_background = m_background;

		m_cleared = false;
	}
}

