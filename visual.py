import networkx as nx
import numpy as np
import matplotlib.pyplot as plt

graph = nx.DiGraph()
adjMatrix = np.loadtxt('./matrix.dat', dtype='int')
graph = nx.from_numpy_array(adjMatrix)
pos= nx.spring_layout(graph)
nx.draw_networkx_nodes(graph, pos, node_size=700)
nx.draw_networkx_edges(graph, pos, width=6)
nx.draw_networkx_labels(graph, pos, font_size=20, font_family='sans-serif')
plt.savefig("graph.png")