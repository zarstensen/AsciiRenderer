#pragma once

#include "Asciir/Rendering/RenderConsts.h"
#include "Asciir/Rendering/AsciiAttributes.h"

namespace Asciir
{
	template<typename TStream>
	void AsciiAttr::ansiCode(TStream& stream, bool is_newline)
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

		if (m_cleared)
			has_changed = true;
		

		if (!has_changed)
			return;
		// cursor
		moveCode(stream);

		// formatting
		stream << AR_ANSIS_CSI;

		if(attributes[BOLD] && (!last_attributes[BOLD] || m_cleared))
			stream << ";1";
		else if(!attributes[BOLD] && (last_attributes[BOLD] || m_cleared))
			stream << ";21";

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
		// background color needs to be updated if the foreground color is changed
		if (m_foreground != m_last_foreground || m_cleared || is_newline)
		{
			stream << ";38;2;";
			stream << std::to_string(m_foreground.red);
			stream << ";";
			stream << std::to_string(m_foreground.green);
			stream << ";";
			stream << std::to_string(m_foreground.blue);

			stream << ";48;2;";
			stream << std::to_string(m_background.red);
			stream << ';';
			stream << std::to_string(m_background.green);
			stream << ';';
			stream << std::to_string(m_background.blue);
		}
		else if (m_background != m_last_background || m_cleared || is_newline)
		{
			// only background color
			stream << ";48;2;";
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
	
	template<typename TStream>
	void AsciiAttr::moveCode(TStream& stream)
	{
		if (m_should_move)
		{
			stream << AR_ANSIS_CSI;
			stream << std::to_string( m_pos.y + 1) << ';' << std::to_string(m_pos.x + 1) << 'f';

			m_should_move = false;
		}
	}
}

