/*
 * fluid simulator
 *
 * a terminal fluid simulator using C#
 *
 * this is my first C# project so excuses for bad code
*/

public class Grid {
	public struct Grid2D {
		public float[,] data;
		public int width, height;
	}

	public static Grid2D create(int width, int height) {
		Grid2D grid = new Grid2D();
		grid.data = new float[width, height];
		return grid;
	}
}
