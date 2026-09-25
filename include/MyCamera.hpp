#pragma once

#include "raylib.h"

class MyCamera {

  public:
    MyCamera();
    void Update(float dt);

    Camera3D& getCamera() { return _camera; }

  private:
    Camera3D _camera;
    // bool _isOrbiting;
    float _th = 0.0f;
    float _orbitRadius = 8.0f;
    float _orbitSpeed = 0.2f;
};
