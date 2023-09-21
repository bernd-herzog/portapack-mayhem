#include "ui_external_items_menu_loader.hpp"

#include "ui_sd_example.hpp"

namespace ui {

std::vector<GridItem> ExternalItemsMenuLoader::load_external_items(app_location_t app_location, NavigationView& nav) {
    // TODO: load listfrom sd card

    if (app_location == app_location_t::UTILITIES) {
        return {{"SD Example", Color::dark_blue(), &bitmap_icon_morse, [&nav]() { nav.push<SdExampleView>(); }}};
    } else {
        return {};
    }
}
}  // namespace ui
