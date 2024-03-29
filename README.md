# MyVox
Lightweight, simple, C++, CPU-based voxelizer based on the implementation of https://github.com/sylefeb/VoxSurf. 
For further details on the algorithm, refer to their repository.

Voxelizer implementation requires no further dependencies. It operates on ```.stl``` files and outputs a ```.csv``` file
which is intented to be processed further with ```Paraview```. 

This voxelizer implementation ouputs the same results as the base implementation of VoxSurf without any further dependencies. 
Additionally, it has the option to scale the voxelization to the same size as the underlying ```.stl``` file.

## Getting Started

Clone the repository to your local machine and to the project build directory. It comes with a ```Makefile```, therefore you only
have run the ```make``` command for installation.

```
git clone https://github.com/alhermann/MyVox.git
cd ~/MyVox/build
make clean && make
```

Make sure to use the correct C++ compiler for your machine. For MacOS change inside the ```Makefile``` to ```clang++```, 
for ```Linux``` machines use ```CC = g++``` (default) and on ```Windows``` machines use ```icpc``` or ```g++``` from the ```MinGW```
compiler.

## Example

Pass an ```.stl``` file path as an argument. The voxelized output will be placed inside the ```build``` directory. 
Optionally pass a 
fourth argument as the voxel resolution.

```cd ~/MyVox/build
./MyVox ../model.stl ../model.csv
```
![STL](./stl_model.png "STL")
![VOX](./vox_model.png "Voxelization") 
