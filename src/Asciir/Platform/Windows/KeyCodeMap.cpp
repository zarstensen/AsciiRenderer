#include "arpch.h"
#include "KeyCodeMap.h"
#include "Log.h"

namespace Asciir
{
    SHORT keyCodeToWin(Key code)
    {

        AR_ASSERT(code != Key::UNKNOWN);

        switch (code)
        {
        case Key::SPACE:
            return VK_SPACE;

        case Key::APOSTROPHE:
            return VK_OEM_7;

        case Key::COMMA:
            return VK_OEM_COMMA;

        case Key::MINUS:
            return VK_OEM_MINUS;

        case Key::PERIOD:
            return VK_OEM_PERIOD;

        case Key::SLASH:
            return VK_OEM_2;

        case Key::_0:
            return 0x30;

        case Key::_1:
            return 0x31;

        case Key::_2:
            return 0x32;

        case Key::_3:
            return 0x33;

        case Key::_4:
            return 0x34;

        case Key::_5:
            return 0x35;

        case Key::_6:
            return 0x36;

        case Key::_7:
            return 0x37;

        case Key::_8:
            return 0x38;

        case Key::_9:
            return 0x39;

        case Key::SEMICOLON:
            return VK_OEM_1;

        case Key::PLUS:
            return VK_OEM_PLUS;

        case Key::A:
            return 0x41;

        case Key::B:
            return 0x42;

        case Key::C:
            return 0x43;

        case Key::D:
            return 0x44;

        case Key::E:
            return 0x45;

        case Key::F:
            return 0x46;

        case Key::G:
            return 0x47;

        case Key::H:
            return 0x48;

        case Key::I:
            return 0x49;

        case Key::J:
            return 0x4A;

        case Key::K:
            return 0x4B;

        case Key::L:
            return 0x4C;

        case Key::M:
            return 0x4D;

        case Key::N:
            return 0x4E;

        case Key::O:
            return 0x4F;

        case Key::P:
            return 0x50;

        case Key::Q:
            return 0x51;

        case Key::R:
            return 0x52;

        case Key::S:
            return 0x53;

        case Key::T:
            return 0x54;

        case Key::U:
            return 0x55;

        case Key::V:
            return 0x56;

        case Key::W:
            return 0x57;

        case Key::X:
            return 0x58;

        case Key::Y:
            return 0x59;

        case Key::Z:
            return 0x5A;

        case Key::LEFT_BRACKET:
            return VK_OEM_4;

        case Key::RIGHT_BRACKET:
            return VK_OEM_6;

        case Key::BACKSLASH:
            return VK_OEM_5;

        case Key::GRAVE_ACCENT:
            return VK_OEM_3;

        case Key::WORLD_1:
            return VK_OEM_8;

        case Key::WORLD_2:
            return VK_OEM_102;

        case Key::ESCAPE:
            return VK_ESCAPE;

        case Key::ENTER:
            return VK_RETURN;

        case Key::TAB:
            return VK_TAB;

        case Key::BACKSPACE:
            return VK_BACK;

        case Key::INSERT:
            return VK_INSERT;

        case Key::DEL:
            return VK_DELETE;

        case Key::RIGHT:
            return VK_RIGHT;

        case Key::LEFT:
            return VK_LEFT;
        
        case Key::DOWN:
            return VK_DOWN;

        case Key::UP:
            return VK_UP;
            
        case Key::PAGE_UP:
            return VK_PRIOR;

        case Key::PAGE_DOWN:
            return VK_NEXT;

        case Key::HOME:
            return VK_HOME;

        case Key::END:
            return VK_END;

        case Key::CAPS_LOCK:
            return VK_CAPITAL;

        case Key::SCROLL_LOCK:
            return VK_SCROLL;

        case Key::NUM_LOCK:
            return VK_NUMLOCK;

        case Key::PRINT_SCREEN:
            return VK_PRINT;

        case Key::PAUSE:
            return VK_PAUSE;

        case Key::F1:
            return VK_F1;

        case Key::F2:
            return VK_F2;

        case Key::F3:
            return VK_F3;

        case Key::F4:
            return VK_F4;

        case Key::F5:
            return VK_F5;

        case Key::F6:
            return VK_F6;

        case Key::F7:
            return VK_F7;

        case Key::F8:
            return VK_F8;

        case Key::F9:
            return VK_F9;

        case Key::F10:
            return VK_F10;

        case Key::F11:
            return VK_F11;

        case Key::F12:
            return VK_F12;

        case Key::F13:
            return VK_F13;

        case Key::F14:
            return VK_F14;

        case Key::F15:
            return VK_F15;

        case Key::F16:
            return VK_F16;

        case Key::F17:
            return VK_F17;

        case Key::F18:
            return VK_F18;

        case Key::F19:
            return VK_F19;

        case Key::F20:
            return VK_F20;

        case Key::F21:
            return VK_F21;

        case Key::F22:
            return VK_F22;

        case Key::F23:
            return VK_F23;

        case Key::F24:
            return VK_F24;

        case Key::KP_0:
            return VK_NUMPAD0;

        case Key::KP_1:
            return VK_NUMPAD1;

        case Key::KP_2:
            return VK_NUMPAD2;

        case Key::KP_3:
            return VK_NUMPAD3;

        case Key::KP_4:
            return VK_NUMPAD4;

        case Key::KP_5:
            return VK_NUMPAD5;

        case Key::KP_6:
            return VK_NUMPAD6;

        case Key::KP_7:
            return VK_NUMPAD7;

        case Key::KP_8:
            return VK_NUMPAD8;

        case Key::KP_9:
            return VK_NUMPAD9;

        case Key::KP_DECIMAL:
            return VK_DECIMAL;

        case Key::KP_DIVIDE:
            return VK_DIVIDE;

        case Key::KP_MULTIPLY:
            return VK_MULTIPLY;

        case Key::KP_SUBTRACT:
            return VK_SUBTRACT;

        case Key::KP_ADD:
            return VK_ADD;

        case Key::KP_ENTER:
            return VK_RETURN;

        case Key::KP_EQUAL:
            return VK_RETURN;

        case Key::LEFT_SHIFT:
            return VK_LSHIFT;

        case Key::LEFT_CONTROL:
            return VK_LCONTROL;

        case Key::LEFT_ALT:
            return VK_LMENU;

        case Key::LEFT_SUPER:
            return VK_LWIN;

        case Key::RIGHT_SHIFT:
            return VK_RSHIFT;

        case Key::RIGHT_CONTROL:
            return VK_RCONTROL;

        case Key::RIGHT_ALT:
            return VK_RMENU;

        case Key::RIGHT_SUPER:
            return VK_RWIN;
        
        case Key::MENU:
            return VK_APPS;
        default:
            AR_ASSERT_MSG(false, "Unknown key: ", (int)code);
            return -1;

        }
    }

    SHORT mouseCodeToWin(MouseKey code)
    {

        AR_ASSERT(code != MouseKey::UNKNOWN);

        switch (code)
        {   
        case MouseKey::LEFT_BUTTON:
            return VK_LBUTTON;
        
        case MouseKey::RIGHT_BUTTON:
            return VK_RBUTTON;
        
        case MouseKey::MIDDLE_BUTTON:
            return VK_MBUTTON;

        case MouseKey::BUTTON_1:
            return VK_XBUTTON1;

        case MouseKey::BUTTON_2:
            return VK_XBUTTON2;
        default:
            AR_ASSERT_MSG(false, "Unknown mouse key: ", (int)code);
            return -1;
        }
    }
}