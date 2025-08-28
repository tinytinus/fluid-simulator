/*
 * Grid 
 * the grid class containing:
 *  the Grid2D structure
 *  the constructor for Grid2D
 *  basic interaction with Grid2D
 */
public class Grid {
	/*
	 * Grid.Grid2D
	 * a grid structure containing:
	 * 	the floats of data 
	 * 	the width
	 * 	the height
	 */
	public struct Grid2D {
		public float[,] data;
		public int width, height;
	}

	/*
	 * Grid.create
	 * creates a new grid 
	 * 	@return Grid2D - returns the newly created grid
	 * 	@param width - the width of the grid to create
	 * 	@param height - the height of the grid to create
	 *
	 * works by: 
	 * 	making a new Grid2D
	 * 	allocating the new data of size width * height
	 * 	storing the width and height
	 * 	returning the new grid
	 */
	public static Grid2D create(int width, int height) {
		Grid2D grid = new Grid2D();
		grid.data = new float[width, height];

		grid.width = width;
		grid.height = height;

		return grid;
	}

	/*
	 * Grid.get
	 * gets the value from a position in a Grid2D
	 * 	@return float - returns the float that was stored at that location 
	 * 	@param grid - the grid to get the data from 
	 * 	@param x - the x position
	 * 	@param y - the y position
	 *
	 * works by:
	 * 	checking if the grid exists
	 * 	clamp the position to stay withing the grid
	 * 	getting the value from that position 
	 * 	returning that value
	 */
	public static float get(Grid2D grid, int x, int y) {
		if (!grid) return 0.0f; 

		if (x > grid.width) x = grid.width;
		if (x < 0) x = 0;
		if (y > grid.height) y = grid.height;
		if (y < 0) y = 0;

		float value = grid.data[x, y];
		return value;
	}

	/*
	 * Grid.set
	 * sets a position on a grid to a value
	 * 	@param grid - the grid to target
	 * 	@param x - the x position
	 * 	@param y - the y position
	 * 	@param value - the value to set the position to 
	 *
	 * works by:
	 * 	checking if the grid exists
	 * 	checking if the position is inside of the grid
	 *	setting the position to the value
	 */
	public static void set(Grid2D grid, int x, int y, float value) {
		if (!grid) return;

		if (x > grid.width || x < 0) return;
		if (y > grid.height || y < 0) return;

		grid.data[x, y] = value;
	}

	/*
	 * Grid.add
	 * adds a value to a position
	 * 	@param grid - the grid to target
	 * 	@param x - the x position
	 * 	@param y - the y position
	 * 	@param value - the value to add to the position
	 *
	 * works by:
	 * 	checking if the grid exists
	 * 	checking if the position is inside of the grid
	 *	setting the position to the value
	 */
	public static void add(Grid2D grid, int x, int y, float value) {
		if (!grid) return;

		if (x > grid.width || x < 0) return;
		if (y > grid.height || y < 0) return;
	
		grid.data[x, y] += value;
	}
}

public class FluidMath {
	/*
	 * lerp
	 * linear interpolation of two values 
	 * 	@retrun float - returns the interpolated value
	 *	@param a - the first value to interpolate
	 *	@param b - the second value to interpolate
	 *	@param t - the strength to interpolate by (0.0 - 1.0)
	 *
	 * works by:
	 * rcalculating the diffrence between a and b and multiplying it by the percentage t 
	 * 	returning the calculated value
	 */
	public static float lerp(float a, float b, float t) {
		float value = a + t * (b - a);
		return value;
	}

	/*
	 * bilerp
	 * bilinear interpolation of a grid at the position (x, y)
	 *	@return float - returns the interpolated value 
	 *	@param grid - the grid to target
	 *	@param x the x position
	 *	@param y the y position
	 *
	 * works by:
	 *	checking if the grid exists
	 *	getting the position and value of the four corners
	 *	getting the strength to interpolate by
	 *	interpolating the top row
	 *	interpolating the bottom row 
	 *	interpolating the top and bottom togheter 
	 *	returning the value 
	 */
	public static float bilerp(Grid2D grid, float x, float y) {
		if (!grid) return 0.0f;

		int left = (int)x;
		int right = left + 1;
		int top = (int)y;
		int bottom = top + 1;

		float sx = x - (float)left;
		float sy = y - (float)top;

		float topLeft = Grid.get(grid, left, top);
		float topRight = Grid.get(grid, right, top);
		float bottomLeft = Grid.get(grid, left, bottom);
		float bottomRight = Grid.get(grid, right, bottom);

		float topInterp = lerp(topLeft, topRight, sx);
		float bottomInterp = lerp(bottomLeft, bottomRight, sx);

		float value = lerp(topInterp, bottomInterp, sy);
		return value;
	}
}

