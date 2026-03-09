import glob
import numpy as np
import matplotlib.pyplot as plt
import os

# Local folder

folder = r"C:\Users\yourname\data\GB_sim"   # Windows example
# folder = "/home/yourname/data/GB_sim"     # Linux/Mac example

files = glob.glob(os.path.join(folder, "*.xyz"))

print(f"Found {len(files)} files")

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


# Loop through files
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
