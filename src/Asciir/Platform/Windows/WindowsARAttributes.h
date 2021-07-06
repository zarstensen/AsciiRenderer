#pragma once
#include "Asciir/Rendering/AsciiAttributes.h"

namespace Asciir
{

	class WinARAttr : public AsciiAttr
	{

		HANDLE m_hConsole;
		DWORD m_fallback_mode;
		UINT m_fallback_cp;

	
	public:
		WinARAttr();
		~WinARAttr() final;

		std::string ansiCode() final;
		void ansiCode(std::string& dst) final;
		void ansiCode(std::ostream& stream, bool is_newline = false) final;
		void ansiCode(TerminalRenderer& dst, bool is_newline = false) final;

		Coord terminalPos() const final;
		TermVert terminalSize() const final;
		TermVert maxTerminalSize() const final;
		Size2D fontSize() const final;
	};
}

