#include <math.h>
#include <stdbool.h>

#include "math_utils.h"

Vec2 vec2_create(float x, float y) { return (Vec2){x, y}; }

Vec2 vec2_add(Vec2 a, Vec2 b) { return (Vec2){a.x + b.x, a.y + b.y}; }

Vec2 vec2_subtract(Vec2 a, Vec2 b) { return (Vec2){a.x - b.x, a.y - b.y}; }

Vec2 vec2_scale(Vec2 v, float scalar) {
  return (Vec2){v.x * scalar, v.y * scalar};
}

float vec2_height(Vec2 v) { return sqrt(v.x * v.x + v.y * v.y); }

Vec2 vec2_normalize(Vec2 v) {
  float len = vec2_height(v);
  if (len != 0) {
    return (Vec2){v.x / len, v.y / len};
  } else {
    return (Vec2){v.x, v.y};
  }
}

float lerp(float a, float b, float t) { return (a + t * (b - a)); }

float bilinear_interpolate(float tl, float tr, float bl, float br, float tx,
                           float ty) {
  float top = lerp(tl, tr, tx);
  float bottom = lerp(bl, br, tx);
  return lerp(bottom, top, ty);
}

float clamp(float value, float min, float max) {
  if (value > max) {
    return max;
  } else if (value < min) {
    return min;
  } else {
    return value;
  }
}
