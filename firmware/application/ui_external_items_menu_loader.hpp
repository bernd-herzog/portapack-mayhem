
#include "ui.hpp"
#include "ui_navigation.hpp"

namespace ui {

enum app_location_t {
    UTILITIES = 0,
    RX,
    TX
};

class ExternalItemsMenuLoader {
   public:
    static std::vector<GridItem> load_external_items(app_location_t, NavigationView&);
    ExternalItemsMenuLoader() = delete;
};

}  // namespace ui
