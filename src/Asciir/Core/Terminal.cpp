#include "arpch.h"
#include "Terminal.h"
#include "Asciir\Logging\Log.h"

#ifdef AR_WIN
#include "Asciir/Platform/Windows/WindowsTerminal.h"
#endif

namespace Asciir
{
	Terminal::Terminal(const TerminalProps& props)
		: m_terminal_render(props.title, props.bufferSize)
	{
		m_terminal_render.resize(props.size);
	}

	std::unique_ptr<Terminal> Terminal::create(const TerminalProps& props)
	{
		#ifdef AR_WIN
		return std::make_unique<WindowsTerminal>(props);
		#else
		assert((false, "Platform not supported"));
		#endif
	}

	
	
	void Terminal::onStart()
	{
		AR_INFO("Started engine");
	}

	TermVert Terminal::getSize() const
	{
		return m_terminal_render.termSize();
	}

	Coord Terminal::getPos() const
	{
		return m_terminal_render.pos();
	}

	TerminalRender* const Terminal::getRenderer()
	{
		return &m_terminal_render;
	}
}
