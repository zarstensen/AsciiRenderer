#pragma once

#include <Windows.h>
#include "Asciir/Input/KeyCodes.h"

namespace Asciir
{
    const std::map<WORD, Key> WinToKeyCodeMap = {
        {VK_SPACE, Key::SPACE},

        {VK_OEM_7, Key::APOSTROPHE},

        {VK_OEM_COMMA, Key::COMMA},

        {VK_OEM_MINUS, Key::MINUS},

        {VK_OEM_PERIOD, Key::PERIOD},

        {VK_OEM_2, Key::SLASH},

        {0x30, Key::_0},

        {0x31, Key::_1},

        {0x32, Key::_2},

        {0x33, Key::_3},

        {0x34, Key::_4},

        {0x35, Key::_5},

        {0x36, Key::_6},

        {0x37, Key::_7},

        {0x38, Key::_8},

        {0x39, Key::_9},

        {VK_OEM_1, Key::SEMICOLON},

        {VK_OEM_PLUS, Key::PLUS},

        {0x41, Key::A},

        {0x42, Key::B},

        {0x43, Key::C},

        {0x44, Key::D},

        {0x45, Key::E},

        {0x46, Key::F},

        {0x47, Key::G},

        {0x48, Key::H},

        {0x49, Key::I},

        {0x4A, Key::J},

        {0x4B, Key::K},

        {0x4C, Key::L},

        {0x4D, Key::M},

        {0x4E, Key::N},

        {0x4F, Key::O},

        {0x50, Key::P},

        {0x51, Key::Q},

        {0x52, Key::R},

        {0x53, Key::S},

        {0x54, Key::T},

        {0x55, Key::U},

        {0x56, Key::V},

        {0x57, Key::W},

        {0x58, Key::X},

        {0x59, Key::Y},

        {0x5A, Key::Z},

        {VK_OEM_4, Key::LEFT_BRACKET},

        {VK_OEM_6, Key::RIGHT_BRACKET},

        {VK_OEM_5, Key::BACKSLASH},

        {VK_OEM_3, Key::GRAVE_ACCENT},

        {VK_OEM_8, Key::WORLD_1},

        {VK_OEM_102, Key::WORLD_2},

        {VK_ESCAPE, Key::ESCAPE},

        {VK_RETURN, Key::ENTER},

        {VK_TAB, Key::TAB},

        {VK_BACK, Key::BACKSPACE},

        {VK_INSERT, Key::INSERT},

        {VK_DELETE, Key::DEL},

        {VK_RIGHT, Key::RIGHT},

        {VK_LEFT, Key::LEFT},

        {VK_DOWN, Key::DOWN},

        {VK_UP, Key::UP},

        {VK_PRIOR, Key::PAGE_UP},

        {VK_NEXT, Key::PAGE_DOWN},

        {VK_HOME, Key::HOME},

        {VK_END, Key::END},

        {VK_CAPITAL, Key::CAPS_LOCK},

        {VK_SCROLL, Key::SCROLL_LOCK},

        {VK_NUMLOCK, Key::NUM_LOCK},

        {VK_PRINT, Key::PRINT_SCREEN},

        {VK_PAUSE, Key::PAUSE},

        {VK_F1, Key::F1},

        {VK_F2, Key::F2},

        {VK_F3, Key::F3},

        {VK_F4, Key::F4},

        {VK_F5, Key::F5},

        {VK_F6, Key::F6},

        {VK_F7, Key::F7},

        {VK_F8, Key::F8},

        {VK_F9, Key::F9},

        {VK_F10, Key::F10},

        {VK_F11, Key::F11},

        {VK_F12, Key::F12},

        {VK_F13, Key::F13},

        {VK_F14, Key::F14},

        {VK_F15, Key::F15},

        {VK_F16, Key::F16},

        {VK_F17, Key::F17},

        {VK_F18, Key::F18},

        {VK_F19, Key::F19},

        {VK_F20, Key::F20},

        {VK_F21, Key::F21},

        {VK_F22, Key::F22},

        {VK_F23, Key::F23},

        {VK_F24, Key::F24},

        {VK_NUMPAD0, Key::KP_0},

        {VK_NUMPAD1, Key::KP_1},

        {VK_NUMPAD2, Key::KP_2},

        {VK_NUMPAD3, Key::KP_3},

        {VK_NUMPAD4, Key::KP_4},

        {VK_NUMPAD5, Key::KP_5},

        {VK_NUMPAD6, Key::KP_6},

        {VK_NUMPAD7, Key::KP_7},

        {VK_NUMPAD8, Key::KP_8},

        {VK_NUMPAD9, Key::KP_9},

        {VK_DECIMAL, Key::KP_DECIMAL},

        {VK_DIVIDE, Key::KP_DIVIDE},

        {VK_MULTIPLY, Key::KP_MULTIPLY},

        {VK_SUBTRACT, Key::KP_SUBTRACT},

        {VK_ADD, Key::KP_ADD},

        {VK_RETURN, Key::KP_ENTER},

        {VK_RETURN, Key::KP_EQUAL},

        {VK_LSHIFT, Key::LEFT_SHIFT},
        
        {VK_SHIFT, Key::LEFT_SHIFT },

        {VK_LCONTROL, Key::LEFT_CONTROL},
        
        {VK_CONTROL, Key::LEFT_CONTROL },

        {VK_LMENU, Key::LEFT_ALT},
       
        {VK_MENU, Key::LEFT_ALT },

        {VK_LWIN, Key::LEFT_SUPER},

        {VK_RSHIFT, Key::RIGHT_SHIFT},

        {VK_RCONTROL, Key::RIGHT_CONTROL},

        {VK_RMENU, Key::RIGHT_ALT},

        {VK_RWIN, Key::RIGHT_SUPER},

        {VK_APPS, Key::MENU},

        { VK_VOLUME_MUTE, Key::VOLUME_MUTE },
        { VK_VOLUME_DOWN, Key::VOLUME_DOWN },
        { VK_VOLUME_UP, Key::VOLUME_UP },
        { VK_MEDIA_STOP, Key::STOP },
        { VK_MEDIA_PREV_TRACK, Key::SKIP_BACKWARD },
        { VK_MEDIA_PLAY_PAUSE, Key::PLAY_PAUSE },
        { VK_MEDIA_NEXT_TRACK, Key::SKIP_FORWARD }
    };

    const std::map<WORD, MouseKey> WinToMouseCodeMap = {
        {FROM_LEFT_1ST_BUTTON_PRESSED, MouseKey::LEFT_BUTTON},

        {RIGHTMOST_BUTTON_PRESSED, MouseKey::RIGHT_BUTTON},

        {FROM_LEFT_2ND_BUTTON_PRESSED, MouseKey::MIDDLE_BUTTON},

        {FROM_LEFT_3RD_BUTTON_PRESSED, MouseKey::BUTTON_1},

        {FROM_LEFT_4TH_BUTTON_PRESSED, MouseKey::BUTTON_2}
    };
}
