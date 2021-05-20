#include "arpch.h"
#include "KeyCodeMap.h"
#include "UnixARAttributes.h"
#include "Asciir/Logging/Log.h"
#include "Asciir/Core/Engine.h"

namespace Asciir
{
    int keyCodeToUnix(Key code)
    {

        AR_ASSERT(code != Key::UNKNOWN);

        int unix_xk;

        switch (code)
        {
        case Key::SPACE:
            unix_xk = XK_space;
            break;

        case Key::APOSTROPHE:
            unix_xk = XK_apostrophe;
            break;

        case Key::COMMA:
            unix_xk = XK_comma;
            break;

        case Key::MINUS:
            unix_xk = XK_minus;
            break;

        case Key::PERIOD:
            unix_xk = XK_period;
            break;

        case Key::SLASH:
            unix_xk = XK_slash;
            break;

        case Key::_0:
            unix_xk = XK_0;
            break;

        case Key::_1:
            unix_xk = XK_1;
            break;

        case Key::_2:
            unix_xk = XK_2;
            break;

        case Key::_3:
            unix_xk = XK_3;
            break;

        case Key::_4:
            unix_xk = XK_4;
            break;

        case Key::_5:
            unix_xk = XK_5;
            break;

        case Key::_6:
            unix_xk = XK_6;
            break;

        case Key::_7:
            unix_xk = XK_7;
            break;

        case Key::_8:
            unix_xk = XK_8;
            break;

        case Key::_9:
            unix_xk = XK_9;
            break;
            
        case Key::SEMICOLON:
            unix_xk = XK_semicolon;
            break;

        case Key::PLUS:
            unix_xk = XK_plus;
            break;

        case Key::A:
            unix_xk = XK_a;
            break;

        case Key::B:
            unix_xk = XK_b;
            break;

        case Key::C:
            unix_xk = XK_c;
            break;

        case Key::D:
            unix_xk = XK_d;
            break;

        case Key::E:
            unix_xk = XK_e;
            break;
            
        case Key::F:
            unix_xk = XK_f;
            break;

        case Key::G:
            unix_xk = XK_g;
            break;

        case Key::H:
            unix_xk = XK_h;
            break;

        case Key::I:
            unix_xk = XK_i;
            break;

        case Key::J:
            unix_xk = XK_j;
            break;

        case Key::K:
            unix_xk = XK_k;
            break;

        case Key::L:
            unix_xk = XK_l;
            break;

        case Key::M:
            unix_xk = XK_m;
            break;

        case Key::N:
            unix_xk = XK_n;
            break;

        case Key::O:
            unix_xk = XK_o;
            break;

        case Key::P:
            unix_xk = XK_p;
            break;

        case Key::Q:
            unix_xk = XK_q;
            break;

        case Key::R:
            unix_xk = XK_r;
            break;

        case Key::S:
            unix_xk = XK_s;
            break;

        case Key::T:
            unix_xk = XK_t;
            break;
            
        case Key::U:
            unix_xk = XK_u;
            break;

        case Key::V:
            unix_xk = XK_v;
            break;

        case Key::W:
            unix_xk = XK_w;
            break;

        case Key::X:
            unix_xk = XK_x;
            break;

        case Key::Y:
            unix_xk = XK_y;
            break;

        case Key::Z:
            unix_xk = XK_z;
            break;

        case Key::LEFT_BRACKET:
            unix_xk = XK_bracketleft;
            break;

        case Key::RIGHT_BRACKET:
            unix_xk = XK_bracketright;
            break;

        case Key::BACKSLASH:
            unix_xk = XK_backslash;
            break;

        case Key::GRAVE_ACCENT:
            unix_xk = XK_grave;
            break;

        case Key::WORLD_1:
            unix_xk = XK_oslash;
            break;

        case Key::WORLD_2:
            unix_xk = 94;
            break;

        case Key::ESCAPE:
            unix_xk = XK_Escape;
            break;

        case Key::ENTER:
            unix_xk = XK_Return;
            break;

        case Key::TAB:
            unix_xk = XK_Tab;
            break;

        case Key::BACKSPACE:
            unix_xk = XK_BackSpace;
            break;

        case Key::INSERT:
            unix_xk = XK_Insert;
            break;

        case Key::DEL:
            unix_xk = XK_Delete;
            break;

        case Key::RIGHT:
            unix_xk = XK_Right;
            break;
            
        case Key::LEFT:
            unix_xk = XK_Left;
            break;

        case Key::DOWN:
            unix_xk = XK_Down;
            break;
            
        case Key::UP:
            unix_xk = XK_Up;
            break;

        case Key::PAGE_UP:
            unix_xk = XK_Page_Up;
            break;

        case Key::PAGE_DOWN:
            unix_xk = XK_Page_Down;
            break;

        case Key::HOME:
            unix_xk = XK_Home;
            break;

        case Key::END:
            unix_xk = XK_End;
            break;

        case Key::CAPS_LOCK:
            unix_xk = XK_Caps_Lock;
            break;

        case Key::SCROLL_LOCK:
            unix_xk = XK_Scroll_Lock;
            break;

        case Key::NUM_LOCK:
            unix_xk = XK_Num_Lock;
            break;

        case Key::PRINT_SCREEN:
            unix_xk = XK_Print;
            break;

        case Key::PAUSE:
            unix_xk = XK_Pause;
            break;

        case Key::F1:
            unix_xk = XK_F1;
            break;

        case Key::F2:
            unix_xk = XK_F2;
            break;

        case Key::F3:
            unix_xk = XK_F3;
            break;

        case Key::F4:
            unix_xk = XK_F4;
            break;

        case Key::F5:
            unix_xk = XK_F5;
            break;

        case Key::F6:
            unix_xk = XK_F6;
            break;

        case Key::F7:
            unix_xk = XK_F7;
            break;

        case Key::F8:
            unix_xk = XK_F8;
            break;

        case Key::F9:
            unix_xk = XK_F9;
            break;

        case Key::F10:
            unix_xk = XK_F10;
            break;

        case Key::F11:
            unix_xk = XK_F11;
            break;

        case Key::F12:
            unix_xk = XK_F12;
            break;

        case Key::F13:
            unix_xk = XK_F13;
            break;

        case Key::F14:
            unix_xk = XK_F14;
            break;

        case Key::F15:
            unix_xk = XK_F15;
            break;

        case Key::F16:
            unix_xk = XK_F16;
            break;

        case Key::F17:
            unix_xk = XK_F17;
            break;

        case Key::F18:
            unix_xk = XK_F18;
            break;

        case Key::F19:
            unix_xk = XK_F19;
            break;

        case Key::F20:
            unix_xk = XK_F20;
            break;

        case Key::F21:
            unix_xk = XK_F21;
            break;

        case Key::F22:
            unix_xk = XK_F22;
            break;

        case Key::F23:
            unix_xk = XK_F23;
            break;

        case Key::F24:
            unix_xk = XK_F24;
            break;

        case Key::KP_0:
            unix_xk = XK_KP_0;
            break;

        case Key::KP_1:
            unix_xk = XK_KP_1;
            break;

        case Key::KP_2:
            unix_xk = XK_KP_2;
            break;

        case Key::KP_3:
            unix_xk = XK_KP_3;
            break;

        case Key::KP_4:
            unix_xk = XK_KP_4;
            break;

        case Key::KP_5:
            unix_xk = XK_KP_5;
            break;

        case Key::KP_6:
            unix_xk = XK_KP_6;
            break;

        case Key::KP_7:
            unix_xk = XK_KP_7;
            break;

        case Key::KP_8:
            unix_xk = XK_KP_8;
            break;

        case Key::KP_9:
            unix_xk = XK_KP_9;
            break;

        case Key::KP_DECIMAL:
            unix_xk = XK_KP_Decimal;
            break;

        case Key::KP_DIVIDE:
            unix_xk = XK_KP_Divide;
            break;

        case Key::KP_MULTIPLY:
            unix_xk = XK_KP_Multiply;
            break;

        case Key::KP_SUBTRACT:
            unix_xk = XK_KP_Subtract;
            break;

        case Key::KP_ADD:
            unix_xk = XK_KP_Add;
            break;

        case Key::KP_ENTER:
            unix_xk = XK_KP_Enter;
            break;

        case Key::KP_EQUAL:
            unix_xk = XK_KP_Equal;
            break;

        case Key::LEFT_SHIFT:
            unix_xk = XK_Shift_L;
            break;

        case Key::LEFT_CONTROL:
            unix_xk = XK_Control_L;
            break;

        case Key::LEFT_ALT:
            unix_xk = XK_Alt_L;
            break;
            
        case Key::LEFT_SUPER:
            unix_xk = XK_Super_L;
            break;

        case Key::RIGHT_SHIFT:
            unix_xk = XK_Shift_R;
            break;

        case Key::RIGHT_CONTROL:
            unix_xk = XK_Control_R;
            break;

        case Key::RIGHT_ALT:
            unix_xk = XK_Alt_R;
            break;

        case Key::RIGHT_SUPER:
            unix_xk = XK_Super_R;
            break;

        case Key::MENU:
            unix_xk = XK_Menu;
            break;

        default:
            AR_ASSERT_MSG(false, "Unknown key: ", (int)code);
            unix_xk = -1;
            break;
        }
        
        return XKeysymToKeycode(dynamic_cast<UnixARAttr&>(AREngine::getEngine()->getTerminal()->getRenderer()->getAttrHandler()).m_display, unix_xk);
    }

    int mouseCodeToUnix(MouseKey code)
    {

        AR_ASSERT(code != MouseKey::UNKNOWN);

        switch (code)
        {   
        case MouseKey::LEFT_BUTTON:
            return 0;
        
        case MouseKey::RIGHT_BUTTON:
            return 1;
        
        case MouseKey::MIDDLE_BUTTON:
            return 2;

        case MouseKey::BUTTON_1:
            return 3;

        case MouseKey::BUTTON_2:
            return 4;
        default:
            AR_ASSERT_MSG(false, "Unknown mouse key: ", (int)code);
            return -1;
        }
    }
}