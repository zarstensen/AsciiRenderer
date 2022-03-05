#pragma once
#include "Asciir/Rendering/AsciiAttributes.h"

namespace Asciir
{
	class UnixARAttr : public AsciiAttr
	{
	public:
		Display* m_display;
		Window m_window;
		Window m_focus_win;
		
		UnixARAttr();
		~UnixARAttr() final;

		std::string ansiCode() final;
		void ansiCode(std::string& dst) final;
		void ansiCode(std::ostream& stream, bool is_newline = false) final;
		void ansiCode(TerminalRenderer& dst, bool is_newline = false) final;

		Coord terminalPos() const final;
		TermVert terminalSize() const final;
		TermVert maxTerminalSize() const final;

		Size2D fontSize() const final { return {}; }
	};
}

