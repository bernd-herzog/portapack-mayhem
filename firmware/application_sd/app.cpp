#include "app.hpp"
#include "ui_widget.hpp"

#include "ch.h"
#include "string.h"

// TODO: integrate https://github.com/MhageGH/esp32_ILI9328_Pacman

extern "C" void app(void) {
    ui::Painter painter;

    for (size_t i = 0; i < 10; i++) {
        painter.fill_rectangle(
            {0, 0, portapack::display.width(), portapack::display.height()},
            ui::Color::blue());

        chThdSleep(500);

        painter.fill_rectangle(
            {0, 0, portapack::display.width(), portapack::display.height()},
            ui::Color::black());

        chThdSleep(500);
    }
}
