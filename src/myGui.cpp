#include "SkyRayConfig.hpp"
#define RAYGUI_IMPLEMENTATION
#include "myGui.hpp"
#include "raygui.h"

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
    float y = _panel.y + 30.0f + gap;
    float w = _panel.width - 2 * pad;

    if (GuiWindowBox(_panel, "SkyRay")) {
        _visible = false;
        DisableCursor();
        return;
    };

    GuiLine({x, y, w, rowH}, "Visual Settings");
    y += rowH + gap;
    GuiCheckBox({x, y, rowH, rowH}, "Debug mode", &SkyRayConfig::SHOW_DEBUG);
    y += rowH + gap;
    GuiLabel({x, y, w, rowH},
             TextFormat("Exposure = 2^%.2f", SkyRayConfig::EXPOSURE));
    y += rowH + gap;
    GuiSlider({x, y, w, rowH}, "", "", &SkyRayConfig::EXPOSURE, -2.0f, 8.0f);
    y += rowH + gap;

    GuiLabel({x, y, w, rowH},
             TextFormat("Camera FOV = %.0f", SkyRayConfig::CAMERA_FOV));
    y += rowH + gap;
    GuiSlider({x, y, w, rowH}, "", "", &SkyRayConfig::CAMERA_FOV, 30, 120);
    y += rowH + gap;

    GuiLine({x, y, w, rowH}, "Simulation Parameters");
    y += rowH + gap;
    // char textBuffer[10] = "1.0";
    // GuiValueBoxFloat({x + 30,y,w / 2, rowH}, "tekst1", textBuffer, &SkyRayConfig::rs, true);
    if(GuiButton({x, y, w / 2 - pad / 2, rowH}, SkyRayConfig::USE_SPHERICAL ? "Spherical" : "Cartesian")){
        SkyRayConfig::USE_SPHERICAL = !SkyRayConfig::USE_SPHERICAL;
    }
    GuiToggle({x + w/2 + pad/2, y, w / 2 - pad / 2, rowH}, "Orbit", &SkyRayConfig::ORBIT);
    y += rowH + gap;

    _panel.height = y - _panel.y + pad;
}
