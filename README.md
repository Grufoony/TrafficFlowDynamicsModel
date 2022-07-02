# Traffic Flow Dynamics Model
A C++ library for generating traffic flow dynamics data using a reference model.

This project is part of a three-year thesis work for the Physics course at the Alma Mater Studiorum - University of Bologna.

## Abstract
*Italiano*

A partire dagli anni ’50 furono sviluppati numerosi modelli con l’intento di studiare i
fenomeni connessi al traffico. Alcuni di questi modelli riuscirono non solo a spiegare i
fenomeni per i quali erano stati ideati ma misero in evidenza altre caratteristiche tipiche
dei sistemi dinamici, come la presenza di cicli di isteresi e cambiamenti nella distribuzione
dei tempi di percorrenza in situazioni di congestione.
Questo lavoro si propone di verificare la validità di un modello semplificato ideato per
evidenziare i comportamenti tipici di un sistema di traffico, in particolare le congestioni
che si vengono a creare sulla rete stradale. Tale modello viene implementato per mezzo della libreria C++ **Traffic Flow Dynamics Model**.
Ai fini dello studio vengono utilizzati i Diagrammi Fondamentali Macroscopici, particolari
diagrammi che mettono in relazione gli osservabili principali di un network stradale quali
velocità, densità e flusso. Variando il carico immesso nella rete stradale è stato possibile
studiare il sistema in diversi regimi: carico costante, carico piccato e carico periodico.
Mediante questi studi sono emerse diverse proprietà tipiche di ogni regime e, per alcuni
di essi, è stata verificate e giustificate la presenza di uno o più cicli di isteresi. In
ultimo è stata effettuata una breve analisi ad-hoc volta a evidenziare i cambiamenti nella
distrubuzione dei tempi di percorrenza in relazione al regime di traffico considerato.

## Compiling
Once created the main file, you can simply compile the source code using the Makefile.
You can compile the program for a linux distribution

    $ make wsl

or for a windows distibution

    $ make windows

## Data
Once run the simulation you can print data in two formats:

1. **latex**, wich prints data in a format readable by the package __pgfplots__ of LaTeX;
2. **root**, wich prints data in a format readable by the C++ framework Root, developed by Cern.

There is also a way to visualize the network in which the streets change color based on their vehicle density.
To do that there is a Python script, __visual.py__ that prints a PNG file of an arbitrary instant of time.
With this script you can also merge differents PNG files into a unique GIF file, obtaining an output like this:
<img src="./data/img/evolution.gif" width="50%" height="50%"/>