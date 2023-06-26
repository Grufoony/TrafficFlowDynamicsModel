# Traffic Flow Dynamics Model
![version](https://img.shields.io/github/v/release/grufoony/TrafficFlowDynamicsModel) [![license](https://img.shields.io/badge/license-GPL--3.0-yellow)](https://github.com/Grufoony/TrafficFlowDynamicsModel/blob/main/LICENSE.md) [![docs](https://img.shields.io/docsrs/regex)](https://grufoony.github.io/TrafficFlowDynamicsModel/)

A C++ library for generating traffic flow dynamics data using a reference model.

This project is part of a three-year thesis work for the Physics course at the Alma Mater Studiorum - University of Bologna.

## Table of contents
1. [Citing](#citing)
2. [Abstract](#abstract)
3. [Download](#download)
4. [Testing](#testing)
5. [Compiling](#compiling)
6. [Data](#data)

## Citing
If you use my code, please cite my work:

Berselli, Gregorio (2022) Modelli di traffico per la formazione della congestione su una rete stradale. [Laurea], Università di Bologna, Corso di Studio in Fisica [L-DM270] <http://amslaurea.unibo.it/view/cds/CDS9244/> 

    @phdthesis{amslaurea26332,
    author = {Gregorio Berselli},
    title = {Modelli di traffico per la formazione della congestione su una rete stradale},
    url = {http://amslaurea.unibo.it/26332/},
    year = {2022}
    }

## Abstract

*English*

Beginning in the 1950s, numerous models were developed with the intent of studying the
traffic related phenomena. Some of them managed not only to explain the
phenomena for which they were designed but highlighted other typical characteristics of
dynamical systems, such as the presence of hysteresis loops and changes in distribution of
travel times in congestion situations.
This work aims to verify the validity of a simplified model designed for
highlight the typical behaviors of a traffic system, in particular congestions
that are created on the road network. This model is implemented by the C ++ **Traffic Flow Dynamics Model** library.
For the analysis, the study uses Macroscopic Fundamental Diagrams, particular
diagrams that relate the main observable of a road network such as
velocity, density and flow. By varying the load introduced into the road network it is possible to
study the system in different regimes: constant load, peaked load and periodic load.
Through these studies, different properties typical of each regimen emerged and, for some
of these, the presence of one or more hysteresis cycles has been verified and justified. Lastly, a brief ad-hoc analysis was carried out aimed at highlighting the changes in the
distribution of travel times in relation to the traffic regime considered.

*Italiano*

A partire dagli anni ’50 furono sviluppati numerosi modelli con l’intento di studiare i
fenomeni connessi al traffico. Alcuni di essi riuscirono non solo a spiegare i
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
di essi, è stata verificata e giustificata la presenza di uno o più cicli di isteresi. In
ultimo è stata effettuata una breve analisi ad-hoc volta a evidenziare i cambiamenti nella
distrubuzione dei tempi di percorrenza in relazione al regime di traffico considerato.

## Download
You can obtain the library downloading the *src* directory from the repository.

Otherwise, you can directly download a release.

## Compiling
To use the library correctly it's better to include at least the Graph and Vehicle headers:
```c++
#include "./src/Graph.hpp"
#include "./src/Vehicle.hpp"
```
also, remember to link all the *.cpp* files when compiling your main, for instance:
```bash
g++ -std=c++20 -O3 ./main.cpp ./src/Graph.cpp ./src/Street.cpp ./src/Vehicle.cpp ./src/VehicleType.cpp
```
You can also try to work with the original *main.cpp* used for the report, building using the make file, like:
```bash
make linux
```
or:
```bash
make windows
```
Then, you can run the analysis you want to perform with the command `make` and one option (`constant`, `peaked`, `periodic`, `traveltime`).
Make sure to uncomment all the necessary and comment the unnecessary into the main file before compiling it.

An easier approach is to run the library on Python keeping the optimization level given by C++.
To make this you have to make the binding of the code on your personal environment.

To do so, in the main folder you can just run:
```bash
./build.sh
```
Then you can run any simulation done in the report running the `example.py` script with one option (`constant`, `peaked`, `periodic`, `traveltime`), for instance:
```bash
python3 example.py peaked
```

## Testing
The test for all the classes are into the *test.cpp* file.
To execute them, just run:
```bash
make test
```
Notice that these tests use [doctest](https://github.com/doctest/doctest).

## Data
Once run the simulation you can print data in two formats:

1. **latex**, which prints data in a format readable by the package [*pgfplots*](https://pgfplots.sourceforge.net/) of LaTeX;
2. **root**, which prints data in a format readable by the C++ framework [*Root*](https://root.cern/), developed by CERN.

There is also a way to visualize the network in which the streets change color based on their vehicle density.
To do that there is a Python script, `visual.py` that prints a PNG file of an arbitrary instant of time.
With this script you can also merge different PNG files into a unique GIF file, obtaining an output like this:

<img src="https://github.com/Grufoony/TrafficFlowDynamicsModel/blob/main/data/img/evolution.gif?raw=true" width="50%" height="50%"/>