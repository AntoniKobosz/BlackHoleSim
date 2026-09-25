#include "SkyRayConfig.hpp"
#define RAYGUI_IMPLEMENTATION
#include "MyGui.hpp"
#include "raygui.h"

#include "raylib.h"

#include "cherry/style_cherry.h"

void MyGui::Init() { GuiLoadStyleCherry(); }

void MyGui::Draw(MyCamera &myCamera, MyGrid grid) {

    if (SkyRayConfig::SHOW_DEBUG)
        DrawDebug(myCamera, grid);
    if (!SkyRayConfig::GUI_VISIBLE)
        return;

    const float pad = 30.0f, rowH = 28.0f, gap = 8.0f, labelW = 90.0f;

    float x = _panel.x + pad;
    float y = _panel.y + 30.0f + gap;
    float w = _panel.width - 2 * pad;

    if (GuiWindowBox(_panel, "SkyRay")) {
        SkyRayConfig::GUI_VISIBLE = false;
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
    // GuiValueBoxFloat({x + 30,y,w / 2, rowH}, "tekst1", textBuffer,
    // &SkyRayConfig::rs, true);
    if (GuiButton({x, y, w / 2 - pad / 2, rowH},
                  SkyRayConfig::USE_SPHERICAL ? "Spherical" : "Cartesian")) {
        SkyRayConfig::USE_SPHERICAL = !SkyRayConfig::USE_SPHERICAL;
    }
    GuiToggle({x + w / 2 + pad / 2, y, w / 2 - pad / 2, rowH}, "Orbit",
              &SkyRayConfig::ORBIT);
    y += rowH + gap;
    GuiLabel({x, y, w, rowH},
             TextFormat("Eps = 1e%.2f", SkyRayConfig::LOG_EPS));
    y += rowH + gap;
    GuiSlider({x, y, w, rowH}, "", "", &SkyRayConfig::LOG_EPS, -5.0f, -1.0f);
    y += rowH + gap;

    _panel.height = y - _panel.y + pad;

    if (SkyRayConfig::SHOW_DEBUG) {

        DrawText(TextFormat("rs = %.1f", SkyRayConfig::rs), 10, 50, 20, LIME);
        DrawText(TextFormat("maxR = %.1f", SkyRayConfig::maxR), 10, 90, 20,
                 LIME);
    }
}

void MyGui::DrawDebug(MyCamera &myCamera, MyGrid grid) {

    if (SkyRayConfig::SHOW_DEBUG) {
        DrawFPS(10, 10);
        // DrawText(TextFormat("Slices = %d", Slices), 10, 90, 20, LIME);
        // DrawText(TextFormat("GridWidth = %.2f", GridWidth), 10, 130, 20,
        //          LIME);
        // DrawText(TextFormat("Subdivs = %d", Subdivs), 10, 170, 20, LIME);
        BeginMode3D(myCamera.getCamera());
        DrawSphere({0, 0, 0}, SkyRayConfig::rs, GOLD);
        DrawSphereWires({0, 0, 0}, SkyRayConfig::rs * 2.6, 8, 9, LIME);
        // DrawSphereWires({0, 0, 0}, SkyRayConfig::maxR, 8, 9, DARKBLUE);
        DrawLine3D({0, 0, -SkyRayConfig::maxR}, {0, 0, SkyRayConfig::maxR},
                   PINK);
        // grid.Draw(grid.GetShader());
        EndMode3D();
    }
}

void MyGui::HandleInput() {

    if (IsKeyPressed(KEY_TAB)) {

        SkyRayConfig::GUI_VISIBLE = !SkyRayConfig::GUI_VISIBLE;

        if (SkyRayConfig::GUI_VISIBLE) {
            EnableCursor();
        } else {
            DisableCursor();
        }
    }

    if (IsKeyPressed(KEY_O))
        SkyRayConfig::SHOW_DEBUG = !SkyRayConfig::SHOW_DEBUG;
    if (IsKeyPressed(KEY_R))
        SkyRayConfig::ORBIT = !SkyRayConfig::ORBIT;
    if (IsKeyDown(KEY_ONE)) {
        SkyRayConfig::EXPOSURE -= SkyRayConfig::EXPOSURE_RATE * GetFrameTime();
    }
    if (IsKeyDown(KEY_TWO)) {
        SkyRayConfig::EXPOSURE += SkyRayConfig::EXPOSURE_RATE * GetFrameTime();
    }
    if (IsKeyDown(KEY_THREE)) {
        SkyRayConfig::maxR /= 1.02;
    }
    if (IsKeyDown(KEY_FOUR)) {
        SkyRayConfig::maxR *= 1.02;
    }

    if (IsKeyPressed(KEY_I)) {
        SkyRayConfig::USE_SPHERICAL = !SkyRayConfig::USE_SPHERICAL;
    }
}
