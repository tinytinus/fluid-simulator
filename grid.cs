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
	 * 	getting the value from that position directly
	 * 	returning that value
	 */
	public static float get(Grid2D grid, int x, int y) {
		if (!grid) return 0.0f; 

		// TODO: this should be clamped

		float value = grid.data[x, y];
		return value;
	}
}
