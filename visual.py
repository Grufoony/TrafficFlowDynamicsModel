import networkx as nx
import matplotlib.pyplot as plt
import pandas as pd
import os
from tqdm import tqdm
from PIL import Image
from PIL import ImageDraw

# clear the img folder
for i in tqdm(range(len([name for name in os.listdir('./img')])-1)):
    path = './img/'+str(i)+'.png'
    if(os.path.exists(path)): 
        os.remove('./img/'+str(i)+'.png')

# create new imgs
for fName in tqdm(os.listdir('./img/data')):

    graph = nx.DiGraph()
    edge_df = pd.read_csv('./img/data/'+fName, sep='\t')

    #edge list from dataframe
    edge_list = zip(edge_df['source'], edge_df['target'])

    # normalize colormap
    vmin = 0
    vmax = edge_df['load'].max()
    norm = plt.Normalize(vmin=vmin, vmax=vmax)

    cmap = plt.cm.get_cmap('RdYlGn').reversed()

    graph.add_edges_from(edge_list)
    pos=nx.kamada_kawai_layout(graph, dist=None, pos=None, weight='weight', scale=1, center=None, dim=2)
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
    limits = plt.axis("off")
    plt.savefig('temp.png')
    plt.close()

    img = Image.open('temp.png')

    # Call draw Method to add 2D graphics in an image
    I1 = ImageDraw.Draw(img)
    
    # Add Text to an image
    I1.text((20, 20), "Time: " + fName.split('.')[0], fill=(0, 0, 0))
    
    # Save the edited image
    img.save('./img/' + fName.split('.')[0] + '.png')
    os.remove('temp.png')

#save into a gif
frames = []
for i in tqdm(range(len([name for name in os.listdir('./img')])-1)):
    new_frame = Image.open('./img/'+str(i)+'.png')
    frames.append(new_frame)

# Save into a GIF file that loops forever
frames[0].save('evolution.gif', format='GIF',
            append_images=frames[1:],
            save_all=True,
            duration=300, loop=0)