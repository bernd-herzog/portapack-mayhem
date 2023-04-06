#include "ui_lua_view.hpp"

namespace ui {

    LuaView::LuaView(
        Context& context,
        const Rect parent_rect
    ) : View { parent_rect },
        context_(context)
    {
        
    }

    Context& LuaView::context() const {
        return context_;
    }

}
