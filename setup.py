from glob import glob
from setuptools import setup, find_packages
from pybind11.setup_helpers import Pybind11Extension, build_ext

__version__ = '1.0.0'

'''
Small script to build the extension module.
'''

ext_modules = [
	Pybind11Extension(
		"TrafficModel",
        ["TrafficModel/binding.cc"]
	),
]

setup(
	name="TrafficModel",
    version=__version__,
	author="Gregorio Berselli, Simone Balducci",
    description="",
	packages=find_packages(),
	# cmdclass={"build_ext": build_ext},
	ext_modules=ext_modules
)
