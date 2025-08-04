
#ifndef MATH_UTILS_H
#define MATH_UTILS_H

// Vector operations
typedef struct {
    float x, y;
} Vec2;

Vec2 vec2_create(float x, float y);
Vec2 vec2_add(Vec2 a, Vec2 b);
Vec2 vec2_subtract(Vec2 a, Vec2 b);
Vec2 vec2_scale(Vec2 v, float scalar);
float vec2_length(Vec2 v);
Vec2 vec2_normalize(Vec2 v);

// Interpolation
float lerp(float a, float b, float t);
float bilinear_interpolate(float tl, float tr, float bl, float br, float tx, float ty);

// Boundary conditions
float apply_boundary(float value, int x, int y, int width, int height);

#endif

