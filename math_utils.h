
#ifndef MATH_UTILS_H
#define MATH_UTILS_H

/*
	Vec2 
	used to store 2 floats of data 
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
	return (Vec2){x,y};
}

/*
   	vec2_add 
   	adds 2 Vec2's togheter 
   	@return Vec2 - returns the added value of a and b
   	@param a - Vec2 a to use
   	@param b - Vec2 b to use
*/
Vec2 vec2_add(Vec2 a, Vec2 b) {
	return (Vec2){a.x + b.x, a.y + b.y};
}

/*
   	vec2_subtract
   	subtracts 2 Vec2's 
   	@return Vec2 - returns the value of a - b
   	@param a - Vec2 a to use
   	@param b - Vec2 b to use
*/
Vec2 vec2_subtract(Vec2 a, Vec2 b){
	return (Vec2){a.x - b.x, a.y - b.y};
}

/*
    vec2_scale
	@return Vec2 - return the Vec2 scaled
	@param v - the Vec2 to scale
	@param scalar - how much to scale by
*/

Vec2 vec2_scale(Vec2 v, float scalar) {
	return (Vec2){v.x * scalar, v.y * scalar};
}

/*
	vec2_height
	calculates the speed of fluid regardless of direction
	doesnt have to be about the speed but good example
	@return float - the speed of the fluid
	@param v - the Vec2 to get the speed from
*/
float vec2_height(Vec2 v) {
	return sqrt(v.x * v.x + v.y * v.y);
}

/*
   	vec2_normalize
   	divide by height with divide by 0 protection
   	@return Vec2 - retruns the value of v divided by height
   	@param v - the Vec2 to divide
*/
Vec2 vec2_normalize(Vec2 v) {
	float len = vec2_height(v);
	if (len != 0) {
		return (Vec2){v.x / len, v.y / len};
	} else {
		return (Vec2){v.x, v.y};
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

/*
	clamp
	clamps a value between min and max
	@return float - value between min and max
	@param value - value to clamp
	@param min - the minimum value 
	@param max - the maximum value
*/
float clamp(float value, float min, float max) {
	if (value > max) {
		return max;
	} else if (value < min) {
		return min;
	} else {
		return value;
	}
}

/*
	is_at_boundary
	checks if this space is at the boundary of the screen
	@return bool - true if it is at the edge of the screen, false otherwise
	@param x - the x position to check
	@param y - the y position to check
	@param width - the width to check 
	@param height - the height to check
*/
bool is_at_boundary(int x, int y, int width, int height) {
	if (x == 0 || x == width-1 || y == 0 || y == height-1) {
		return true;
	} else {
		return false;
	}
}

/*
	apply_velocity_boundary
	aplies the velocity changing at the edges \
	@return Vec2 - returns the velocity field 
	@param velocity - the velocity field to change 
	@param x - the x position to look at
	@param y - the y position to look at
	@param width - the width of the field
	@param height - the height of the field
*/
Vec2 apply_velocity_boundary(Vec2 velocity, int x, int y, int width, int height) {
	Vec2 result = velocity;

	if (x == 0 || x == width -1) {
		result.x = 0.0f;
	}

	if (y == 0 || y == height -1) {
		result.y = 0.0f;
	}
	
	return result;
}

#endif

