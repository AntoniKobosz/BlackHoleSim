#include "MyCamera.hpp"
#include "SkyRayConfig.hpp"
#include "raylib.h"
#include "raymath.h"

MyCamera::MyCamera() {

    _camera.position = {7, 1, 7};
    _camera.up = {0, 1, 0};
    _camera.projection = CAMERA_PERSPECTIVE;
    _camera.target = {0, 0, 0};
    _camera.fovy = SkyRayConfig::CAMERA_FOV;
}

void MyCamera::Update(float dt) {

    _camera.fovy = SkyRayConfig::CAMERA_FOV;
    if (SkyRayConfig::ORBIT) {
        if (IsKeyDown(KEY_A))
            _orbitSpeed += _angAccel * dt;
        if (IsKeyDown(KEY_D))
            _orbitSpeed -= +_angAccel * dt;
        if (IsKeyDown(KEY_SPACE)) {
            _th -= _ThVel * dt;
            if (_th < 0)
                _th = 0;
        }
        if (IsKeyDown(KEY_LEFT_CONTROL)) {
            _th += _ThVel * dt;
            if (_th > PI)
                _th = PI;
        }
        if (IsKeyDown(KEY_W)) {
            _orbitRadius -= _Rvel * dt;
            if(_orbitRadius <= SkyRayConfig::rs) _orbitRadius = SkyRayConfig::rs;
        }

        if (IsKeyDown(KEY_S)) {
            _orbitRadius += _Rvel * dt;
        }

        _phi += _orbitSpeed * dt;
        _camera.position = {cosf(_phi) * sinf(_th) * _orbitRadius,
                            _orbitRadius * cosf(_th),
                            sinf(_phi) * sinf(_th) * _orbitRadius};
        _camera.target = {0, 0, 0};
        return;
    }

    if (!SkyRayConfig::GUI_VISIBLE) {
        UpdateCamera(&_camera, CAMERA_FREE);
    }
}
