#include "SkyRayConfig.hpp"
#define RAYGUI_IMPLEMENTATION
#include "myGui.hpp"
#include "raylib.h"

#include "cherry/style_cherry.h"
void myGui::Init() { GuiLoadStyleCherry(); }

void myGui::HandleInput() {

    if (IsKeyPressed(KEY_TAB)) {

        _visible = !_visible;

        if (_visible) {
            EnableCursor();
        } else {
            // _prevMousePos = GetMousePosition();
            DisableCursor();
        }
    }
}

void myGui::Draw() {

    if (!_visible)
        return;

    const float pad = 30.0f, rowH = 28.0f, gap = 8.0f, labelW = 90.0f;
    float x = _panel.x + pad;
    float y = _panel.y + 30.0f;
    float w = _panel.width - 2 * pad;

    GuiPanel(_panel, "SkyRay");

    GuiLabel({x, y, labelW, rowH}, "Settings");
    y += rowH + gap;
    GuiCheckBox({x, y, rowH, rowH}, "Debug mode", &SkyRayConfig::SHOW_DEBUG);
    y += rowH + gap;

    _panel.height = y - _panel.y + pad;
}
