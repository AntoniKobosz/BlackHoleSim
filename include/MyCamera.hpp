#pragma once

#include "raylib.h"

class MyCamera {

  public:
    MyCamera();
    void Update(float dt);

    Camera3D &getCamera() { return _camera; }

  private:
    Camera3D _camera;
    // bool _isOrbiting;
    const float _angAccel = 0.2;
    const float _ThVel = 0.4;
    const float _Rvel = 7.0;
    float _phi = 0.0f;
    float _th = DEG2RAD * 85.0f;
    float _orbitRadius = 8.0f * 3;
    float _orbitSpeed = 0.2f;
};
