#pragma once

#ifndef AR_WIN
#error Only windows is currently supported
#endif

#ifdef AR_WIN
#define AR_ANSIS_CSI "\x1b["
#define AR_ANSIS_OSC "\x1b]"
#endif

