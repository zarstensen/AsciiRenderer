#pragma once

#include "Asciir/Input/EventListener.h"

namespace Asciir
{
namespace ELInterface
{

    class MacEventListener: public EventListenerImpl
    {
    public:

        void start(EventCallbackFp) {}
        void stop() {}
    };

}

typedef ELInterface::MacEventListener EventListener;

}
