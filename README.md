
# Graphene Grain Boundary Multislice Simulation

## 1. Prerequisites
Ensure the following Python libraries are installed in your environment:
* **NumPy**: Matrix operations and Fast Fourier Transforms (FFT).
* **Matplotlib**: Image visualization and PNG export.
* **Glob & OS**: File handling and path management.

## 2. Workflow

### Phase 1: Data Preparation
* **File Placement**: Move your `.xyz` files into the `/MultiSlice_final_experiment` or `//MultiSlice_final_simulation` directories.
* **Data Format**: 
* **Simulation**: 
    * **Line 1**: `misorientation` and `line_angle` (metadata).
    * **Line 2**: Box dimensions ($S_x, S_y, S_z$).
    * **Line 3+**: Atomic coordinates ($x, y, z$).
* **Experiment**: 
    * **Line 1**: Traces the origin of the data to an experimental .dm3 file (Gatan DigitalMicrograph format), likely a graphene (Gr) * *
    * **Line 2**: PixelSize (0.093734 Å): The critical resolution used for grid discretization.BoundaryLengthEstimate: The physical length of the grain boundary being analyzed.Coords in Å: Confirms that all spatial values are in Angstroms ($10^{-10}$ m).
    * **Line 3+**: Atomic coordinates ($x, y, z$).

* **Setup**: Update the `folder` path variable in the script to match your local directory.


### Phase 2: Physics & Grid Setup
* **Voltage**: Set to **80kV** by default (to prevent knock-on damage in graphene).
* **Sampling**: **0.05 Å/pixel**.
* **FFT Optimization**: Grid dimensions ($N_x, N_y$) are automatically adjusted to the nearest **Power of 2** (e.g., 512, 1024) to optimize computational speed.

### Phase 3: Multislice Calculation
1.  **Potential Projection**: Converts discrete atomic coordinates into a continuous 2D electrostatic potential map.
2.  **Propagation**: The electron wave "steps" through the sample, alternating between real-space phase shifts and Fourier-space transmission.

### Phase 4: Image Formation (CTF)
The script simulates the objective lens using the **Contrast Transfer Function (CTF)** formula:
$$\chi(k) = \pi \lambda \Delta f k^2 + \frac{\pi}{2} C_s \lambda^3 k^4$$

It generates a comparison plot for three defocus conditions:
* **In-Focus (0 Å)**: Pure phase contrast, usually appearing "flat."
* **Scherzer Defocus**: The "sweet spot" where spherical aberration and defocus cancel out to produce maximum structural contrast.
* **2x Scherzer**: Significant delocalization; used to observe oscillatory behavior.

## 3. Output & Export
* **Auto-Save**: The script detects the **Scherzer Defocus** iteration and saves it automatically.
* **Naming Convention**: `thetam_{mis}_thetal_{angle}.png` 
    * *Example*: `thetam_13.0_thetal_4.0.png`
* **Format**: Pure grayscale PNG using `plt.imsave`. This exports the raw intensity matrix without axes or labels, ideal for quantitative analysis.

