#pragma once

#include "ui_widget.hpp"

namespace luaui {
    class Button : public ui::Button {
        public:
            inline int lua_SetText(lua_State *L){
                auto text = luaL_checkstring(L, 2);
                this->set_text(text);
                return 0;
        }
    };
}