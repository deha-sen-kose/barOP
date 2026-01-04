<img width="1003" height="217" alt="Screenshot from 2026-01-04 21-54-40" src="https://github.com/user-attachments/assets/6378b47c-3be6-427e-9bd3-074336ebba69" />

# barOP — Truss Analysis & Shape Optimization Toolkit

barOP is an open-source C++ project focused on structural analysis and shape optimization for truss systems. 
The project currently supports linear elastic analysis and 3D visualization, with planned expansion toward full shape optimization workflows.
The C++ class structure is created such that in the future it will be easier to add new types of elements, such as beams, plates and even shells.

This is a hobby project of mine and I am aware that there are many more softwares/libraries out there that does what barOP is currently capable of achieving and much more.
The underlying reason why I created this project is to learn how to simulate structural problems and conduct optimization using C++. 
That is why I am only using external libraries for unit testing (Gtest) and visualization (VTK). All other operations and computations are carried only using the standard C++ library, and the libraries I wrote myself.

barOP currently includes:

* A custom dynamic templated Matrix library, designed for numerical operations used in FEM, e.g., row and column deletion/insertion, Cholesky decomposition and lower triangular inversion algortihm.
* Several classes working together to perform linear elastic structural analysis for 2D/3D truss systems.
* Polymorphic functions and inherited class structure that will hopefully allow for creation of new types of elements.
* Visualization of truss systems using [VTK](https://vtk.org/), with color grading and color bar to visualize engineering strain and stress fields.
* Unit tests created using [googletest](https://github.com/google/googletest) to ensure that the results are equivalent to the benchmarks.
* [Doxygen](https://www.doxygen.nl/) compatible documentation in header files.
* And, last but not least, a build system using [CMake](https://cmake.org/) to ensure cross-platform compatibility.

<img width="1496" height="600" alt="BarOP2025-11-30" src="https://github.com/user-attachments/assets/0424cd3d-2768-4d62-a79e-70da4709e215" />
Above example radio tower is taken from: Fox, R. L. and Schmit, L. A., Advances in the integrated approach to structural synthesis, AIAA/ASME
Material Conference, 1964.

## Prerequisite
* [CMake](https://cmake.org/) to build
* In order to visualize the truss systems as shown in above picture, [The Visualization Toolkit](https://vtk.org/)
* If you wish to build with unit tests, then you need [googletest](https://github.com/google/googletest)

## Building the project
```
git clone https://github.com/deha-sen-kose/barOP.git
cd barOP
mkdir build && cd build
cmake .. <options>
make
```
## Example Usage
Creating a truss structure (instance) called t1:
```
TrussStructure t1;
```
Adding nodes to the structure, with x, y, and z-coordinates:
```
Node& n1 = t1.addNode(0.0 ,0.0 ,0.0);
```
Adding a material with name "steel":
```
Material& mat = t1.addMaterial("steel", 200E9);
```
Connecting nodes with elements (in this example between nodes n1 and n3, with created material and cross-section area of 2.00):
```
TrussElement& e1 = t1.addTrussElement(n1, n3, mat, 2.00);
```
Adding homogeneous boundary conditions to the system using degrees of freedom notation (first: x, second: y, last: z):
```
std::vector<int> homdof = {1,2,3};
t1.addBCs(homdof);
```
Adding forces on dof and their magnitudes:
```
std::vector<int> forceDof = {8,14,20,26,32};
std::vector<double> forces = {-10, -10, -16, -10, -10};
t1.addForces(forceDof, forces);
```
Solving the linear system of equations for complete displacement vector:
```
std::vector<double> u = t1.solveTrussSystem();
```
To start visualization structure:
```
TrussVisualization vis(t1);
```
Adding letters to indicate fixed dof:
```
vis.addFixedJoints(t1);
```
Adding the scalar field (as it is constant through the element) to visualize with colors (e.g. stress):
```
vis.updateScalarField(t1, u, "stress");
```
To start the visualization window:
```
vis.start();
```

## Roadmap
* Sensitivity analysis and gradient computation
* Shape optimization (e.g., nodal position optimization)
* Cross-section optimization
* Visualization Enhancements
* More complex elements in the future...

## References
* Felippa, C. (2014). *Introduction to Finite Element Methods*
* [VTK Examples](https://examples.vtk.org/site/Cxx/)
* Cholesky-Banachiewicz algorithm from [link](https://algowiki-project.org/en/Cholesky_method)
* ChatGPT for writing unit tests, debugging and help with VTK

## Contact 
To contact: dehasenkose@gmail.com
