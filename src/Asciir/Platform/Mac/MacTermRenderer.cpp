#include "MacTermRenderer.h"

namespace Asciir
{
namespace TRInterface
{
    TermVert termSize() const
    {
        struct winsize wsize;
        AR_VALIDATE(ioctl(0, TIOCGWINSZ, &wsize), "Failed to get terminal size");

        return TermVert(wsize.ws_row, wsize.ws_col);
    }
}
}