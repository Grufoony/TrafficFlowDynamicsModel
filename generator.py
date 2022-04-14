import networkx as nx
import numpy as np

G = nx.grid_2d_graph(10,12)
mat = nx.adjacency_matrix(G)
with open('matrix.dat','wb') as f:
    for line in mat.todense():
        np.savetxt(f, line, fmt='%.2f')
