import TrafficModel
from TrafficModel import Vehicle
from tqdm import tqdm
import matplotlib
matplotlib.use('TkAgg')
import matplotlib.pyplot as plt
import os
import glob

def clear():
    files = glob.glob("./temp_data/*")
    for f in files:
        os.remove(f)

def plot():
    file = open("./temp_data/k-t.dat", "r")
    lines = file.readlines()
    file.close()
    x = []
    y = []
    for line in lines:
        x.append(float(line.split()[0]))
        y.append(float(line.split()[1]))
    plt.plot(x, y)
    plt.show()

# Constant analysis
def constant():
    graph = TrafficModel.Graph("./data/matrix.dat")
    Vehicle.addVehicleType("./data/vehicletype.dat")
    graph.setTemperature(300)
    graph.updateTransMatrix()
    for t in tqdm(range(15001)):
        if t % 100 == 0:
            graph.fprintTimeDistribution("./temp_data/", "k", 0.)
        if (t < 12e3) and (t % 60 == 0) and (t != 0):
            graph.addVehiclesUniformly(250)
        graph.evolve(False)

# Peaked analysis
def peaked():
    graph = TrafficModel.Graph("./data/matrix.dat")
    Vehicle.addVehicleType("./data/vehicletype.dat")
    graph.setTemperature(300)
    graph.updateTransMatrix()
    for t in tqdm(range(12000)):
        if t % 100 == 0:
            graph.fprintTimeDistribution("./temp_data/", "k", 0.)
        if (t < 500) and (t % 50 == 0) and (t != 0):
            graph.addVehiclesUniformly(2500)
        if t < 0.4e4:
            graph.evolve()
        else:
            graph.evolve(False)



if __name__ == "__main__":
    clear()
    constant()
    plot()