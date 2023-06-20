import TrafficModel as tfdm
from tqdm import tqdm
import matplotlib
matplotlib.use('TkAgg')
import matplotlib.pyplot as plt
import os
import glob
import math
import sys

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

def hist(filename):
    file = open(filename, "r")
    lines = file.readlines()
    file.close()
    x = []
    y = []
    for line in lines:
        x.append(float(line.split()[0]))
        y.append(float(line.split()[1]))
    plt.bar(x, y)
    plt.show()

# Constant analysis
def constant():
    clear()
    graph = tfdm.Graph("./data/matrix.dat")
    tfdm.Vehicle.addVehicleType("./data/vehicletype.dat")
    graph.setTemperature(300)
    graph.updateTransMatrix()
    for t in tqdm(range(15001)):
        if t % 25 == 0:
            graph.fprintTimeDistribution("./temp_data/", "k", 0.)
        if (t < 12e3) and (t % 60 == 0) and (t != 0):
            graph.addVehiclesUniformly(250)
        graph.evolve(False)
    plot()

# Peaked analysis
def peaked():
    clear()
    graph = tfdm.Graph("./data/matrix.dat")
    tfdm.Vehicle.addVehicleType("./data/vehicletype.dat")
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
    plot()

# Periodic analysis
def periodic():
    clear()
    graph = tfdm.Graph("./data/matrix.dat")
    tfdm.Vehicle.addVehicleType("./data/vehicletype.dat")
    graph.setTemperature(300)
    graph.updateTransMatrix()
    for t in tqdm(range(55000)):
        if t % 900 == 0:
            graph.fprintTimeDistribution("./temp_data/", "k", 0.)
        if (t % 300 == 0) and (t < 32400 * 1.5):
            vehicles = abs(2200 * math.sin(2 * math.pi * t / 32400))
            if (t > 1.62e4) and (t < 2.43e4):
                vehicles = vehicles / 1.125
            graph.addVehiclesUniformly(int(vehicles))
        graph.evolve(False)
    plot()

# Traveltime analysis
def traveltime():
    clear()
    graph = tfdm.Graph("./data/matrix.dat")
    tfdm.Vehicle.addVehicleType("./data/vehicletype.dat")
    graph.setTemperature(300)
    graph.updateTransMatrix()
    for t in tqdm(range(15001)):
        if t % 100 == 0:
            graph.fprintHistogram("./temp_data/", "traveltime", 100, "latex")
        if (t % 60 == 0) and (t != 0):
            graph.addRndmVehicles(200)
            graph.addVehiclesUniformly(int(200 / 1.125))
        graph.evolve(False)
    hist("./temp_data/3000_t.dat")
    hist("./temp_data/6000_t.dat")
    hist("./temp_data/9000_t.dat")
    hist("./temp_data/12000_t.dat")
    hist("./temp_data/15000_t.dat")

if __name__ == "__main__":
    if len(sys.argv) > 1:
        todo = sys.argv[1].lower()
        if todo == "constant":
            constant()
        elif todo == "peaked":
            peaked()
        elif todo == "periodic":
            periodic()
        elif todo == "traveltime":
            traveltime()