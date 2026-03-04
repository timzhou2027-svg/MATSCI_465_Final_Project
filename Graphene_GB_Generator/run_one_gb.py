import grainBdr as gb
import numpy as np
import ase.visualize as ase_viz

# =============================================================================
# 1. MODEL CONFIGURATION (Key Parameters for your Proposal)
# =============================================================================

# Define the chirality vectors (n, m) for the two grains.
# These determine the Misorientation Angle (theta).
# Example: [1, 2] and [2, 1] results in a ~21.8 degree symmetric GB.
N1_VECTOR = np.array([1, 2])  # Left grain orientation
N2_VECTOR = np.array([2, 1])  # Right grain orientation

# Simulation domain settings
CELL_WIDTH = 120.0     # Total width in Angstroms (Use >100 for 4D-STEM simulations)
BOND_LENGTH = 1.42     # Carbon-Carbon bond length (Experimental standard: 1.42 A)

# Accuracy and Convergence
STRAIN_TOLERANCE = 1e-5 # Max allowed artificial strain to match periodic boundaries
ITER_VERBOSE = True     # Set to True to monitor CVT relaxation progress in terminal

# Output file naming
FILE_PREFIX = "graphene_gb_21.8deg"

# =============================================================================
# 2. CORE EXECUTION (Calling the CVT Algorithm)
# =============================================================================

print(f"--- Starting GB Generation ---")
print(f"Chirality: N1={N1_VECTOR}, N2={N2_VECTOR}")
print(f"Target Width: {CELL_WIDTH} A")

# Generate the 1-Periodic Grain Boundary using the CVT algorithm
# This function automatically creates the 5-7 ring pairs at the interface.
atoms_model = gb.onePeriodicGB(
    N1=N1_VECTOR,
    N2=N2_VECTOR,
    cell_width=CELL_WIDTH,
    a=BOND_LENGTH,
    ep_max=STRAIN_TOLERANCE,
    verbose=ITER_VERBOSE
)

# =============================================================================
# 3. DATA EXPORT & VISUALIZATION
# =============================================================================

# Export to XYZ format: Standard for electron scattering/multislice simulation
xyz_file = f"{FILE_PREFIX}.xyz"
atoms_model.write(xyz_file)
print(f"Successfully saved XYZ coordinates to: {xyz_file}")

# Export to LAMMPS format: Useful for further MD energy minimization (AIREBO potential)
lammps_file = f"{FILE_PREFIX}.lammps"
gb.writeLammpsData(atoms_model, lammps_file)
print(f"Successfully saved LAMMPS data to: {lammps_file}")

# Optional: Interactive 3D visualization using ASE
# This will open a window to inspect the 5-7 ring connections at the GB core.
print("Opening 3D visualizer...")
# ase_viz.view(atoms_model) 

print("--- Task Completed ---")