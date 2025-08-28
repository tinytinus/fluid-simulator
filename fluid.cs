/*
 * FluidSim
 * the FluidSim class containing:
 * 	the FluidSystem structure 
 * 	the constructor and destructor of the fluidsystem 
 */
public class FluidSim {
	/*
	 * FluidSim.FluidSystem
	 * a fluid system structure containing:
	 * 	the width and height
	 * 	the diffusion rate
	 * 	the viscosity rate
	 * 	the time step
	 * 	the pressure array
	 * 	the density array
	 * 	the velocity x array
	 * 	the velocity y array
	 * 	the previous velocity x array
	 * 	the previous velocity y array
	 */
	public struct FluidSystem {}
		public int width, height;
		public float diffusion;
		public float viscosity;
		public float dt;
		public float[] pressure;
		public float[] density;
		public float[] Vx, Vy;
		public float[] VxPrev, VyPrev;
	}

	/*
	 * FluidSim.FluidCreate
	 * creates a new fluidsystem
	 *	@return FluidSystem - returns the created fluid
	 *	@param width - the width of the fluid
	 *	@param height - the height of the fluid
	 *	@param diffusion - the diffusion rate of the fluid
	 *	@param viscosity - the viscosity of the fluid
	 *	@param dt - the time steps
	 *
	 * works by:
	 * 	allocatting the memory
	 * 	checking if the allocatting failed
	 * 	setting the paramaters
	 * 	allocatting the arrays 
	 * 	checking if any allocations failed
	 * 	returning the newly created fluidsystem 
	 */
	public static FluidSystem fluidCreate(int width, int height, float diffusion, float viscosity, float dt) {
		FluidSystem fs = new FluidSystem();
		if (!fs) return null;

		fs.width = width;
		fs.height = height;
		fs.diffusion = diffusion;
		fs.viscosity = viscosity;
		fs.dt = dt;
		int size = (width + 2) * (height + 2);
		fs.pressure = new float[size];
		fs.density = new float[size];
		fs.Vx = new float[size];
		fs.Vy = new float[size];
		fs.VxPrev = new float[size];
		fs.VyPrev = new float[size];
		
		if (!fs.pressure || !fs.density || !fs.Vx || !fs.Vy || !fs.VxPrev || !fs.VyPrev) {
			fluidDestroy(fs);
			return null;
		}

		return fs;
	}

	/*
	 * FluidSim.FluidDestroy
	 * destroys a fluidsystem
	 *	@param fs - the fluid system to destroy
	 */
	fluidDestroy(FluidSystem fs) {
		if (fs) {
			if (fs.pressure) fs.pressure = null;
			if (fs.density) fs.density = null;
			if (fs.Vx) fs.Vx = null;
			if (fs.Vy) fs.Vy = null;
			if (fs.VxPrev) fs.VxPrev = null;
			if (fs.VyPrev) fs.VyPrev = null;
			fs = null;
		}
	}

	/*
	 * FLuidSim.addVelocity
	 * adds velocity to a fluidsystem 
	 * 	@param x - the x position
	 * 	@param y - the y position
	 * 	@param vx - the x velocity
	 * 	@param vy - the y velocity
	 *
	 * works by:
	 * 	checking if the fluidsystem exists
	 * 	checking if its a valid position 
	 * 	adding the x velocity
	 * 	adding the y velocity
	 */
	public static void addVelocity(FluidSystem fs, int x, int y, float vx, float vy) {
		if (!fs) return;
		if (x < 1 || x > fs.width || y < 1 || y > fs.height) return;

		Grid.add(fs.Vx, x, y, vx);
		Grid.add(fs.Vy, x, y, vy);
	}		
