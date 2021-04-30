#pragma once

#include <Windows.h>
#include "Asciir/Input/KeyCodes.h"

namespace Asciir
{
    SHORT keyCodeToWin(Key code);

    SHORT mouseCodeToWin(MouseKey code);
}
