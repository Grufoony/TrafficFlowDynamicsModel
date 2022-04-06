import networkx as nx
import matplotlib.pyplot as plt
import pandas as pd
import os
from tqdm import tqdm
import glob
from PIL import Image

for i in tqdm(os.listdir('./img')):
    os.remove('./img/'+i)

for fName in tqdm(os.listdir('./data')):

    graph = nx.DiGraph()
    edge_df = pd.read_csv('./data/'+fName, sep='\t')

    #edge list from dataframe
    edge_list = zip(edge_df['source'], edge_df['target'])

    # normalize colormap
    vmin = 0
    vmax = edge_df['load'].max()
    norm = plt.Normalize(vmin=vmin, vmax=vmax)

    cmap = plt.cm.get_cmap('RdYlGn').reversed()

    graph.add_edges_from(edge_list)
    pos=nx.spring_layout(graph, seed=69)
    fig, ax = plt.subplots()
    nx.draw_networkx_nodes(graph, pos, ax=ax)
    nx.draw_networkx_labels(graph, pos, ax=ax)
    curved_edges = [edge for edge in graph.edges() if reversed(edge) in graph.edges()]
    temp = []
    # reorder dataframe based on graph_edges
    for edge in graph.edges():
        temp.append(edge_df[(edge_df['source'] == edge[0]) & (edge_df['target'] == edge[1])]['load'].values[0])

    nx.draw_networkx_edges(graph, pos, ax=ax, edgelist=curved_edges, connectionstyle=f'arc3, rad = {0.1}',edge_color=temp,
                    edge_cmap=cmap)

    plt.savefig('img/' + fName.split('.')[0] + '.png')
    plt.close()

frames = []
imgs = glob.glob("./img/*.png")
for i in imgs:
    new_frame = Image.open(i)
    frames.append(new_frame)
 
# Save into a GIF file that loops forever
frames[0].save('evolution.gif', format='GIF',
               append_images=frames[1:],
               save_all=True,
               duration=300, loop=0)