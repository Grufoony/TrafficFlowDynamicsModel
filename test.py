import TrafficModel
from TrafficModel import Vehicle
from tqdm import tqdm
import matplotlib
matplotlib.use('TkAgg')
import matplotlib.pyplot as plt
import os
import glob
import math

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

# Periodic analysis
def periodic():
    graph = TrafficModel.Graph("./data/matrix.dat")
    Vehicle.addVehicleType("./data/vehicletype.dat")
    graph.setTemperature(300)
    graph.updateTransMatrix()
    for t in tqdm(range(55000)):
        if t % 900 == 0:
            graph.fprintTimeDistribution("./temp_data/", "k", 0.)
        if (t % 300 == 0) and (t < 32400 * 1.5):
            vehicles = abs(2100 * math.sin(2 * math.pi * t / 32400))
            if (t > 1.62e4) and (t < 2.43e4):
                vehicles = vehicles * 1.125
            graph.addVehiclesUniformly(int(vehicles))
        graph.evolve(False)



if __name__ == "__main__":
    clear()
    periodic()
    plot()