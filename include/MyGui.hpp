#pragma once

#include "MyCamera.hpp"
#include "MyGrid.hpp"
#include "raylib.h"

class MyGui {

  public:
    void Draw(MyCamera &myCamera, MyGrid grid);
    void HandleInput();
    void Init();

  private:
    void DrawDebug(MyCamera &myCamera, MyGrid grid);
    Rectangle _panel = {128.0f, 128.0f, 400.0f, 128.0f};
};
