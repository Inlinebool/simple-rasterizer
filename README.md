# A Simple C++ Rasterizer

## Project Description

Author: Kairong Jiang

This project implements a resterizing renderer and implements flat, Gouraud and Phong shading models. User can select between the three at run time.

A custom scene file ```myscene.txt```  is provided to illustrated the functionality of the renderer. ```myscene.ppm``` is the result of rending the scene using Phong shading. (All resource files credited to Dr. Joshua Levine.)

## User Instructions

### System Requirements

* CMake 2.8 or newer
* C++ compiler with C++11 support (gcc 4.8.1 or newer, or Clang 3.3 or newer)
* SDL2 library

### Build and Run
Build the program using cmake.

Run the program with the relative path to the scene txt file set as program argument.

e.g. 

```mkdir build```

```cd build```

```cmake ..```

```make```

```./simple_rasterizer ../myscene.txt```

The program will show the image rendered by the renderer. By default, flat shading is used. User can perform the following keyboard operations:

* press ```f``` to switch to flat shading.
* press ```g``` to switch to Gouraud shading.
* press ```p``` to switch to Phong shading.
* press ```s``` to save the image.
   