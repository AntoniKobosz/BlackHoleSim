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
        _th += _orbitSpeed * dt;
        _camera.position = (Vector3){cosf(_th), 0, sinf(_th)} * _orbitRadius;
        _camera.target = {0, 0, 0};
        return;
    }

    if (!SkyRayConfig::GUI_VISIBLE) {
        UpdateCamera(&_camera, CAMERA_FREE);
    }
}
