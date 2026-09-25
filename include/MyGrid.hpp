#pragma once

#include "raylib.h"
struct MyGrid {
  public:
    MyGrid(int Slices, float Width, int bendsPerEdge = 0) {
        slices = Slices;
        width = Width;
        bendsPerEdge = bendsPerEdge;
    }
    int slices;
    float width;
    int bendsPerEdge = 0;

    void Draw();
    void Draw(Shader shader);
    void Update();

    Shader GetShader() { return shader; }

  private:
    Shader shader = LoadShader("resources/shaders/gridShdr.vs",
                               "resources/shaders/gridShdr.fs");
};
