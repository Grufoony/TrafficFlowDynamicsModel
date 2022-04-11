import numpy as np
import matplotlib.pyplot as plt
from sklearn import datasets

data = np.loadtxt("./data/0.dat", delimiter="\t", dtype='float',, skiprows=1, usecols=None)

v_hist = np.ravel(datasets)   # 'flatten' v
img = plt.figure()
ax1 = img.add_subplot(111)

# n, bins, patches = ax1.hist(v_hist, bins=25, normed=1, facecolor='green')
img.save('./img/' + '0' + '.png')