from glob import glob
from setuptools import setup, find_packages
from pybind11.setup_helpers import Pybind11Extension, build_ext

ext_modules = [
	Pybind11Extension(
		"TrafficModel",
		sorted(glob("source/*.cpp"))
	),
]

setup(
	name="TrafficModel",
	author="Gregorio Berselli, Simone Balducci",
	packages=find_packages(),
	cmdclass={"build_ext": build_ext},
	ext_modules=ext_modules
)
