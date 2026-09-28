#pragma once
#include "raylib.h"

namespace SkyRayConfig {

inline float rs = 1.0;
inline float maxR = rs * 60.0;
static constexpr float EXPOSURE_RATE = 3;
inline bool SHOW_DEBUG = false;
inline bool USE_SPHERICAL = false;
inline float EXPOSURE = 0.8f;
inline float CAMERA_FOV = 70;
inline bool ORBIT = false;
inline float LOG_EPS = -3.5f;
inline bool GUI_VISIBLE = false;
inline bool RENDER_DISK = true;
inline float DISK_INNER_RADIUS = 2.3f;  // relative to rs
inline float DISK_OUTER_RADIUS = 12.0f; // relative to rs
inline float DISK_TEMP_FACTOR = 4.6;    // szukamy lepszej
inline float DISK_INTENSITY_FACTOR = 2000;

} // namespace SkyRayConfig
