#pragma once

#include "raygui.h"
#include "raylib.h"


class myGui {

  public:
    void Draw();
    void HandleInput();
    void Init();
    bool IsVisible() { return _visible; }

  private:
    bool _visible = false;
    Rectangle _panel = {128.0f, 128.0f, 500.0f, 0.0f};
};
