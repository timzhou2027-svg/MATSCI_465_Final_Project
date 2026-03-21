
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

# Graphene Grain Boundary Bloch-Wave Analysis

## 1. Prerequisites
Ensure the following Python libraries are installed in your environment:

* **NumPy**: Matrix operations, nearest-neighbor calculations, and linear algebra for strain and orientation mapping.  
* **Matplotlib**: Visualization of band contrast, strain maps, and defect locations.  
* **SciPy (cKDTree)**: Efficient nearest-neighbor search for lattice orientation and strain computations.  
* **NetworkX**: Graph-based ring detection and bond graph traversal.  
* **OS & Glob**: File handling and path management.

---

## 2. Workflow

### Phase 1: Data Preparation
* **File Placement**: Place `.xyz` files into the `/BlochWave_pipeline` directory.  
* **Data Format**:  
  * **Line 1**: Optional metadata (misorientation, line angle).  
  * **Line 2**: Box dimensions ($S_x, S_y, S_z$).  
  * **Line 3+**: Atomic coordinates ($x, y, z$ in Å).  
* **Setup**: Update the `folder` path variable in the script to match your local directory.  

---

### Phase 2: Lattice Orientation & Neighbor Analysis
1. **KD-Tree Construction**: Finds k-nearest neighbors for each atom to calculate bond vectors.  
2. **Bond Angles**: Computes $\phi_{ij} = \arctan2(y_j - y_i, x_j - x_i)$ for each neighbor pair.  
3. **Local Orientation**: Averages bond angles over neighbors to estimate $\theta_i$ for band contrast and excitation error calculations.  

---

### Phase 3: Band Contrast Calculation
* **Excitation Error**:  
$$s_g = \alpha \, \theta_i$$  
Maps local lattice misorientation to deviation from Bragg condition.  
* **Effective Extinction Distance**:  
$$\xi_{\mathrm{eff}} = \frac{\xi_g}{\sqrt{1 + (s_g \xi_g)^2}}$$  
* **Diffracted Intensity**:  
$$I_g = \sin^2 \left( \frac{\pi t}{\xi_{\mathrm{eff}}} \right)$$  
* **Band Contrast**:  
$$C = | I_g - 0.5 |$$  

---

### Phase 4: Strain Mapping & Elastic Energy
1. Compute local displacement vectors $\mathbf{u}_{ij} = \mathbf{r}_{ij} - \langle \mathbf{r}_{ij} \rangle$.  
2. Fit deformation gradient tensor $\mathbf{F}$ to approximate local linear strain.  
3. Calculate symmetric strain tensor:  
$$\boldsymbol{\varepsilon} = \frac{1}{2} (\mathbf{F} + \mathbf{F}^\top)$$  
4. Estimate local elastic energy:  
$$\gamma_i = \frac{1}{2} k \, \mathrm{Tr}(\boldsymbol{\varepsilon}_i^2) d_0^2$$  

---

### Phase 5: Defect Ring Detection
1. Convert atomic coordinates into a **bond graph** using nearest neighbors.  
2. Traverse the graph to extract **closed cycles**, classify by size, and detect defect rings (e.g., 5–7 pairs).  
3. Visualize ring positions over the band contrast or strain map.

---

## 3. Output & Export
* **Band Contrast Maps**: Grayscale images over $(x, y)$ coordinates.  
* **Strain Energy Maps**: Visual heatmaps of local elastic energy.  
* **Defect Overlay**: Rings (5–7, etc.) plotted on top of band contrast or strain maps.  
* **File Naming Convention**:  
`thetam_{mis}_thetal_{angle}_band.png`  
*Example*: `thetam_13.0_thetal_4.0_band.png`  
* **Format**: PNG images for visualization; numerical arrays can be exported via `np.save` for further analysis.
