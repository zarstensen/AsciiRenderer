#include "MacTermRenderer.h"

namespace Asciir
{
namespace TRInterface
{
    TermVert MacTerminalRenderer::termSize() const
    {
        struct winsize wsize;
        AR_VERIFY_MSG(ioctl(0, TIOCGWINSZ, &wsize), "Failed to get terminal size");

        return TermVert(wsize.ws_row, wsize.ws_col);
    }
}
}