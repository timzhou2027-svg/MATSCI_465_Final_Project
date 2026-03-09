import numpy as np
import matplotlib.pyplot as plt
from scipy.spatial import cKDTree
import networkx as nx

# Build bond graph

def build_graph(atoms, cutoff=1.8):

    xy = atoms[:, :2]
    tree = cKDTree(xy)

    pairs = tree.query_pairs(cutoff)

    G = nx.Graph()

    for i in range(len(xy)):
        G.add_node(i, pos=xy[i])

    for i, j in pairs:
        G.add_edge(i, j)

    return G

# Sort neighbors around each atom (needed for faces)

def sort_neighbors(G):

    for n in G.nodes():

        x0, y0 = G.nodes[n]["pos"]

        nbrs = list(G.neighbors(n))

        angles = []

        for m in nbrs:
            x, y = G.nodes[m]["pos"]
            angles.append(np.arctan2(y - y0, x - x0))

        nbrs = [x for _, x in sorted(zip(angles, nbrs))]

        G.nodes[n]["ordered_nbrs"] = nbrs

# Face (ring) detection

def find_faces(G):

    visited = set()
    faces = []

    for u, v in G.edges():

        for direction in [(u, v), (v, u)]:

            if direction in visited:
                continue

            face = []
            start = direction
            current = direction

            while True:

                visited.add(current)

                a, b = current
                face.append(a)

                nbrs = G.nodes[b]["ordered_nbrs"]

                i = nbrs.index(a)
                next_node = nbrs[(i - 1) % len(nbrs)]

                current = (b, next_node)

                if current == start:
                    break

            faces.append(face)

    return faces

# Run detection

G = build_graph(atoms)

sort_neighbors(G)

faces = find_faces(G)

# remove duplicates
rings = []

for f in faces:
    s = tuple(sorted(f))
    if s not in rings:
        rings.append(s)

rings = [list(r) for r in rings]

# classify
counts = {5:0,6:0,7:0}

for r in rings:
    if len(r) in counts:
        counts[len(r)] += 1

print("5 rings:", counts[5])
print("6 rings:", counts[6])
print("7 rings:", counts[7])
degrees = [len(list(G.neighbors(n))) for n in G.nodes()]
print("Min degree:", min(degrees))
print("Max degree:", max(degrees))
five = [r for r in rings if len(r)==5][0]
seven = [r for r in rings if len(r)==7][0]

print("Shared atoms:", set(five).intersection(seven))

plt.figure(figsize=(6,6))

plt.scatter(atoms[:,0], atoms[:,1], s=1, c='gray')

for r in rings:

    pts = atoms[r]

    if len(r) == 5:
        plt.scatter(pts[:,0], pts[:,1], s=15, c='red')

    if len(r) == 7:
        plt.scatter(pts[:,0], pts[:,1], s=15, c='blue')

plt.gca().set_aspect('equal')
plt.title("5–7 Ring Defects")
plt.ylim(0,120)
plt.show()