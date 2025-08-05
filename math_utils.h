
#ifndef MATH_UTILS_H
#define MATH_UTILS_H

/*
   Vec2 
   uesd to store 2 floats of data 
   x - float x, doesnt have to be releated to the x axis
   y - float y, doesnt have to be releated to the y axis
*/
typedef struct {
    float x, y;
} Vec2;

/*
   vec2_create
   creates a Vec2
   @return Vec2 - returns teh created Vec2
   @param x - value for the Vec2's x
   @param y - value for the Vec2's y
*/
Vec2 vec2_create(float x, float y) {
	return {x,y};
}

/*
   vec2_add 
   adds 2 Vec2's togheter 
   @return Vec2 - returns the added value of a and b
   @param a - Vec2 a to use
   @param b - Vec2 b to use
*/
Vec2 vec2_add(Vec2 a, Vec2 b) {
	if (a && b) {
		return {a.x + b.x, a.y + b.y};
	}
}

/*
   vec2_subtract
   subtracts 2 Vec2's 
   @return Vec2 - returns the value of a - b
   @param a - Vec2 a to use
   @param b - Vec2 b to use
*/
Vec2 vec2_subtract(Vec2 a, Vec2 b){
	if (a && b) {
		return {a.x - b.x, a.y - b.y};
	}
}

/*
    vec2_scale
	@return Vec2 - return the Vec2 scaled
	@param v - the Vec2 to scale
	@param scalar - how much to scale by
*/

Vec2 vec2_scale(Vec2 v, float scalar) {
	if (v) {
		return {v.x * scalar, v.y * scalar};
	}
}

/*
	vec2_length
	calculates the speed of fluid regardless of direction
	doesnt have to be about the speed but good example
	@return float - the speed of the fluid
	@param v - the Vec2 to get the speed from
*/
float vec2_length(Vec2 v) {
	if (v) {
		return sqrt(v.x * v.x + v.y * v.y);
	}
}

/*
   vec2_normalize
   divide by length with divide by 0 protection
   @return Vec2 - retruns the value of v divided by length
   @param v - the Vec2 to divide
*/
Vec2 vec2_normalize(Vec2 v) {
	if (v) {
float len = vec2_length(v);
		if (len != 0) {
			return {v.x / len, v.y / len};
		} else {
			return {v.x, x.y};
		}
	}
}

/*
   lerp
   blends linearly between two values
   @return float - returns the value of (a + t * (b - a)) 
   @param a - the data a 
   @param b - the data b 
   @param t - how much of a or b should be taken for the final value 
*/
float lerp(float a, float b, float t) {
	return (a + t * (b - a));
}

/*
   bilinear_interpolate
   lerp but for 4 values instead of 2 
   @return float - returns the blended value of the 4 values
   @param tl - top left
   @param tr - top right
   @param bl - bottom left
   @param br - bottom right
   @param tx - where the point is located on the x axis
   @param ty - where the point is located on the y axis
*/
float bilinear_interpolate(float tl, float tr, float bl, float br, float tx, float ty) {
	float top = lerp(tl, tr, tx);
	float bottom = lerp(bl, br, tx);
	return lerp(bottom, top, ty);
}

// Boundary conditions
bool is_at_boundary(int x, int y, int width, int height);
float apply_boundary(float value, int x, int y, int width, int height);

// general purpose 
float clamp(float value, float min, float max);

#endif

