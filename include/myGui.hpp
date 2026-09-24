#pragma once

#include "raylib.h"


class myGui {

  public:
    void Draw();
    void HandleInput();
    void Init();
    bool IsVisible() { return _visible; }

  private:
    bool _visible = false;
    Rectangle _panel = {128.0f, 128.0f, 400.0f, 128.0f};
};
