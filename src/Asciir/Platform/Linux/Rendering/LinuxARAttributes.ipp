#pragma once

#include "Asciir/Rendering/RenderConsts.h"

namespace Asciir
{
	template<typename TStream>
	void AsciiAttr::ansiCode(TStream& stream)
	{
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

		if (!has_changed)
			return;

		// formatting
		stream << AR_ANSIS_CSI;

		if (attributes[BOLD] && !last_attributes[BOLD])
			stream << ";1";
		else if (!attributes[BOLD] && last_attributes[BOLD])
			stream << ";21"

		if (attributes[ITALIC] && !last_attributes[ITALIC])
			stream << ";3";
		else if (!attributes[ITALIC] && last_attributes[ITALIC])
			stream << ";23";

		if (attributes[UNDERLINE] && !last_attributes[UNDERLINE])
			stream << ";4";
		else if (!attributes[UNDERLINE] && last_attributes[UNDERLINE])
			stream << ";24";

		if (attributes[BLINK] && !last_attributes[BLINK])
			stream << ";5";
		else if (!attributes[BLINK] && last_attributes[BLINK])
			stream << ";25";

		if (attributes[STRIKE] && !last_attributes[STRIKE])
			stream << ";9";
		else if (!attributes[STRIKE] && last_attributes[STRIKE])
			stream << ";29";

		// foreground color
		if (m_foreground != m_last_foreground)
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
		if (m_background != m_last_background)
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
	}
}

