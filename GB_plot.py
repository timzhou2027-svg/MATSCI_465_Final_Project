import glob
import numpy as np
import matplotlib.pyplot as plt
import os

# Local folder

folder = r"C:\Users\yourname\data\GB_sim"   # Windows example
# folder = "/home/yourname/data/GB_sim"     # Linux/Mac example

files = glob.glob(os.path.join(folder, "*.xyz"))

print(f"Found {len(files)} files")

# Read simulated GB files
def read_gb_xyz(filename):

    with open(filename) as f:
        lines = f.readlines()

    # header
    misorientation, line_angle = map(float, lines[0].split())
    box = list(map(float, lines[1].split()))

    coords = []

    for line in lines[2:]:
        if line.strip() == "":
            continue
        
        parts = line.split()
        x, y, z = map(float, parts[:3])
        coords.append([x, y, z])

    coords = np.array(coords)

    return misorientation, line_angle, box, coords

# Read experimental GB files
def read_exp_gb_xyz(filename):

    # 1. Parse angles from filename
    
    m = re.search(r'tM=([\d.]+)deg_tL=([\d.]+)deg', filename)

    misorientation = float(m.group(1))
    line_angle = float(m.group(2))

    # 2. Read coordinates

    coords = []

    with open(filename) as f:
        lines = f.readlines()

    for line in lines:

        parts = line.split()

        # coordinate lines have exactly 3 numbers
        if len(parts) == 3:

            try:
                x, y, z = map(float, parts)
                coords.append([x, y, z])
            except:
                pass

    coords = np.array(coords)

    return misorientation, line_angle, coords


# Loop through files (change gb/exp_gb as needed)
for file in files:

    mis, angle, box, atoms = read_gb_xyz(file)

    print(f"\nFile: {os.path.basename(file)}")
    print("Misorientation:", mis)
    print("Atoms:", atoms.shape)

    offset = 55

    plt.figure(figsize=(6,6))
    plt.scatter(atoms[:,0], atoms[:,1], s=1, c='k')
    plt.gca().set_aspect('equal', adjustable='box')

    plt.xlim(-5,55)
    plt.ylim(-5+offset,25+offset)

    plt.title(f"Graphene Grain Boundary\n{os.path.basename(file)}")
    plt.show()
