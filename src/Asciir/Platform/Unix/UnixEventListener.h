#pragma once

#include "Asciir/Input/EventListener.h"

namespace Asciir
{
namespace ELInterface
{
    class TerminalEventHandler;

    template<>
    class EventListenerImpl<IMPLS::MAC>: public EventListenerImpl<IMPLS::INTER>
    {
    public:

        void start(EventCallbackFp callback)
        {}
		
        void stop()
        {}

        static Coord getCurrentMousePos()
        {
            return {};
        }
    };
}
}