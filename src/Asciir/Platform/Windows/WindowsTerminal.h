#pragma once

#include "Asciir/Core/Terminal.h"

#ifdef AR_WIN
namespace Asciir
{
	class WindowsTerminal : public Terminal
	{
		
	public:
		WindowsTerminal(const TerminalProps& props = TerminalProps());
		~WindowsTerminal();

		void onStart() final;
		void onUpdate() final;
		void updateInput() final;
		
		void setEventCallback(const EventCallbackFp& callback) final;
		void EventCallback(Event& e);
	private:
		TRUpdateInfo m_update_info;
	};
	
	void onCloseSignal(int signal);
}
#endif
