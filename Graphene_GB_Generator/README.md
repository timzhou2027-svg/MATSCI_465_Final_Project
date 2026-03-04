Cite as
* Large-scale experimental and theoretical study of graphene grain boundary structures, Colin Ophus, Ashivni Shekhawat, Haider Rasool, and Alex Zettl, Phys. Rev. B 92, 205402

* Toughness and strength of nanocrystalline graphene, A Shekhawat, RO Ritchie - Nature communications, 2016

Report any bugs/issues to shekhawat.ashivni@gmail.com


# Installation

Compile the c extension with the following command

python setup.py build_ext --inplace

This should result in two modules named _cGBUtils.so and _cPolyUtils.so

# Testing

A sample use of the code is provided in the file test.py Run it with the 
following command

python test.py

This should create the following files

testGB_1Periodic.cfg
testGB_1Periodic.pdb
testGB_1Periodic.lammps

testGB_2Periodic.cfg
testGB_2Periodic.pdb
testGB_2Periodic.lammps

testPoly_2Periodic.cfg
testPoly_2Periodic.pdb
testPoly_2Periodic.lammps

which contain the generated 1 periodic GB, 2 periodic GB, and the polycrystal in cfg, pdb and lammps data format, respectively. 




# User Guide: Graphene GB Generation Script (Customized for MatSci 465)

This user guide describes a **Graphene Grain Boundary (GB) Generator** adapted for our **MatSci 465** course project. Our team has borrowed the core algorithmic framework from **Ophus et al. (2015)** and customized the driver implementation to support our comparative analysis of 4D-STEM and electron scattering simulations.

## 1. Project Context & Attribution
* **Original Source**: The underlying CVT (Centroidal Voronoi Tessellation) algorithm is based on: *Ophus, C., et al., "Large-scale experimental and theoretical study of graphene grain boundary structures," Phys. Rev. B 92, 205402 (2015).*
* **Our Implementation**: We have developed a custom driver script (`run_one_gb.py`) to interface with the original `grainBdr.py` library. We modified the parameter handling and output formatting to better suit the requirements of kinematic and dynamical scattering calculations.

---
## 1. Prerequisites

Before running the script, ensure the following files are located in the **same directory**:
* `run_one_gb.py`: Your main driver script (the "controller").
* `grainBdr.py`: The core library containing the GB construction logic.
* `_cGBUtils.pyd` (Windows) or `_cGBUtils.so` (Linux/Mac): The compiled C++ acceleration module.
* **Dependencies**: Ensure `numpy`, `scipy`, and `ase` are installed in your Python environment.
# Recommended: install specific versions via pip
* pip install numpy==1.26.4 scipy==1.15.3 ase==3.27.0 abTEM==1.0.8 py4DSTEM==0.14.18 matplotlib==3.10.8

---

## 2. Essential Parameters (The "Cheat Sheet")

You only need to modify the following variables in the **CONFIGURATION** section of `run_one_gb.py`:

| Parameter | Type | Description |
| :--- | :--- | :--- |
| **`N1_VECTOR`** | `np.array([n, m])` | **Left Grain Chirality**: Determines the crystallographic orientation of the left crystal. |
| **`N2_VECTOR`** | `np.array([n, m])` | **Right Grain Chirality**: Determines the crystallographic orientation of the right crystal. |
| **`CELL_WIDTH`** | `float` | **Simulation Width**: The total distance (in Å) between boundaries. Set to **>100** for better 4D-STEM diffraction contrast. |
| **`BOND_LENGTH`** | `float` | **C-C Bond Distance**: Use **1.42** for standard experimental graphene models. |
| **`STRAIN_TOLERANCE`** | `float` | **Numerical Accuracy**: Lower values (e.g., `1e-6`) reduce artificial lattice stretching but increase computation time. |

---

## 3. Common GB Orientations (Reference Table)

## How to Convert Misorientation Angle ($\theta$) to Input Vectors ($N_1, N_2$)

In our generator, the grain orientation is defined by the chirality vector $(n, m)$.  
To match a specific misorientation angle $\theta$ reported in the Ophus *et al.* paper, use the following conversion procedure.

---

### 1. Chirality Angle Formula ($\chi$)

For a grain defined by the vector $(n, m)$, the chirality angle $\chi$ relative to the zigzag direction ($0^\circ$) is

$$
\chi = \arctan \left( \frac{\sqrt{3}\, m}{2n + m} \right)
$$

---

### 2. Misorientation Angle ($\theta$)

For a **symmetric grain boundary**, the indices of the two grains are swapped:

$$
N_1 = [n, m], \qquad N_2 = [m, n]
$$

The total misorientation angle $\theta$ between the two grains is then

$$
\theta = 2 \times \left| 30^\circ - \chi \right|
$$

---

### Summary

Given $(n,m)$:

1. Compute $\chi$ using the chirality formula.
2. Swap indices to construct symmetric grains.
3. Compute the misorientation angle:

$$
\theta = 2|30^\circ - \chi|
$$

To match the experimental data from **Ophus et al. (2015)**, use these preset vector pairs for symmetric grain boundaries:

* **$\theta \approx 21.8^\circ$**: Set `N1=[1, 2], N2=[2, 1]`
* **$\theta \approx 32.2^\circ$**: Set `N1=[1, 3], N2=[3, 1]`
* **$\theta \approx 13.2^\circ$**: Set `N1=[2, 3], N2=[3, 2]`



---

## 4. Workflow (How to run)

1.  **Modify**: Open `run_one_gb.py` in your editor and set your desired `N1_VECTOR`, `N2_VECTOR`, and `CELL_WIDTH`.
2.  **Execute**: Run the script in your terminal:
    ```bash
    python run_one_gb.py
    ```
3.  **Monitor**: Watch the `Max Update` value in the terminal. The CVT algorithm is converging when this number drops below your `STRAIN_TOLERANCE` (e.g., `1e-5`).
4.  **Verify Output**:
    * **`.xyz` file**: Use this for your 4D-STEM **multislice simulations** (Kinematic/Dynamical scattering).
    * **`.lammps` file**: Use this if you plan to perform further **energy minimization** in LAMMPS (e.g., using AIREBO potential).



---

## 5. Technical Note
The script uses a geometric **Centroidal Voronoi Tessellation (CVT)** approach to automatically relax the interface. This process naturally generates the **5-7 ring pairs** (dislocation cores) required for a physically realistic representation of graphene grain boundaries as described in your project proposal.