import networkx as nx
import numpy as np
import matplotlib.pyplot as plt

graph = nx.DiGraph()
adjMatrix = np.loadtxt('./matrix.dat', dtype='int')
graph = nx.from_numpy_array(adjMatrix)
options = ['r','b']
colors = []
for i in range(len(graph.edges())):
    if(i%2 == 0):
        colors.append(options[0])
    else:
        colors.append(options[1])
pos= nx.spring_layout(graph)
nx.draw_networkx_nodes(graph, pos, node_size=550)
nx.draw_networkx_edges(graph, pos, edge_color=colors, width=2)
nx.draw_networkx_labels(graph, pos, font_size=12, font_family='sans-serif')
plt.savefig("graph.png")