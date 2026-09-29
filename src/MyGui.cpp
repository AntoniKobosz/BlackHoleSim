#include "SkyRayConfig.hpp"
#define RAYGUI_IMPLEMENTATION
#include "MyGui.hpp"
#include "raygui.h"

#include "cmath"
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
    GuiSlider({x + (w + pad) / 2, y, (w - pad) / 2, rowH}, "", "",
              &SkyRayConfig::EXPOSURE, -2.0f, 8.0f);
    y += rowH + gap;

    GuiLabel({x, y, w, rowH},
             TextFormat("Camera FOV = %.0f", SkyRayConfig::CAMERA_FOV));
    GuiSlider({x + (w + pad) / 2, y, (w - pad) / 2, rowH}, "", "",
              &SkyRayConfig::CAMERA_FOV, 30, 120);
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
    GuiSlider({x + (w + pad) / 2, y, (w - pad) / 2, rowH}, "", "",
              &SkyRayConfig::LOG_EPS, -6.0f, -1.0f);
    y += rowH + gap;
    GuiLine({x, y, w, rowH}, "Accretion disk");
    y += rowH + gap;
    GuiCheckBox({x, y, rowH, rowH}, "Render disk", &SkyRayConfig::RENDER_DISK);
    y += rowH + gap;
    GuiLabel({x, y, w, rowH}, TextFormat("Inner radius = %.2f rs",
                                         SkyRayConfig::DISK_INNER_RADIUS));
    GuiSlider({x + w / 2 + pad / 2, y, (w - pad) / 2, rowH}, "", "",
              &SkyRayConfig::DISK_INNER_RADIUS, 1.0f,
              std::fmin(SkyRayConfig::DISK_OUTER_RADIUS, 3.0));

    y += rowH + gap;
    GuiSlider({x + w / 2 + pad / 2, y, (w - pad) / 2, rowH}, "", "",
              &SkyRayConfig::DISK_OUTER_RADIUS, SkyRayConfig::DISK_INNER_RADIUS,
              24.0f);
    GuiLabel(
        {x, y, (w - pad) / 2, rowH},
        TextFormat("Outer radius = %.2f rs", SkyRayConfig::DISK_OUTER_RADIUS));

    y += rowH + gap;

    GuiLabel({x, y, w, rowH},
             TextFormat("Peak T = %.0f K",
                        pow(10.0f, SkyRayConfig::DISK_TEMP_FACTOR) * 0.488));
    GuiSlider({x + (w + pad) / 2, y, (w - pad) / 2, rowH}, "", "",
              &SkyRayConfig::DISK_TEMP_FACTOR, 3.31158f, 4.91364f);
    y += rowH + gap;
    GuiLabel({x, y, w, rowH}, TextFormat("Noise strength = %.2f",
                                         SkyRayConfig::DISK_NOISE_STRENGTH));
    GuiSlider({x + (w + pad) / 2, y, (w - pad) / 2, rowH}, "", "",
              &SkyRayConfig::DISK_NOISE_STRENGTH, 0.0f, 0.5f);
    y += rowH + gap;

    GuiLabel({x, y, w, rowH}, TextFormat("Swirl = %.2f",
                                         SkyRayConfig::DISK_SWIRL));
    GuiSlider({x + (w + pad) / 2, y, (w - pad) / 2, rowH}, "", "",
              &SkyRayConfig::DISK_SWIRL, -0.3f, 0.3f);
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
    if (IsKeyPressed(KEY_U))
        SkyRayConfig::RENDER_DISK = !SkyRayConfig::RENDER_DISK;
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
