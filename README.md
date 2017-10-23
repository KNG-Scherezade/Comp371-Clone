# comp-371-procedural-generation-project

Loads a raster image and renders a 3D surface by mapping brightness at a given location to surface height at the corresponding location.

Works on macOS and Linux. Should work for Windows, but not recently tested.

**You may get the best results if you use CLion as your IDE - but other IDEs should work.**

## Acknowledgements

### Dependencies

Beyond the C++ standard library this application relies on:
* OpenGL
* GLFW
* GLEW
* GLM
* STB (particularly the stb_image.h header library)

(Following standard setup procedure in a CMake-compatible IDE should fetch all these dependencies for you, so you shouldn't need to download them separately.)

### Other help

* Starter skeleton code adapted from [LearnOpenGL.com](https://learnopengl.com/) (including inspiration for [camera direction movement](https://learnopengl.com/#!Getting-started/Camera) and COMP 371 at Concordia University
* Cross platform CMake setup created using [Hunter](https://github.com/ruslo/hunter)
* Camera direction mouse movement partially modeled after tutorial
* Thanks to the COMP 371 TAs and my classmates for helping me think through some of these tougher problems

## Features

* Load a 2D heightmap image and render it as a 3D model
* Swap between different model variations at the tap of a button
* View the model from any camera angle

### Starting the app

1. Before a window appears, you will be prompted to enter an image filename to load. If you don't care, a default will be used.
2. As soon as the program performs the initial 3D model render, the rendering will pause and you will need to enter a skip size for point reduction.
3. You will also need to enter a step size for point interpolation frequency.
4. Now you can explore the world generated by your heightmap.

### Controls

* Press 1, 2, 3 or 4 to view:
    1. The initial model rendered directly from the heightmap
    2. The reduced-point model
    3. The reduced-point model with added interpolated points along the x axis
    4. The previous model, with full interpolation along the z axis as well
* Press P to switch to points mode, L to switch to lines mode and T to switch to triangles mode
* Use WASD or the arrow keys to move the player forward, back, left or right
* Click the window with the left mouse button to capture the mouse and enter camera look mode
* Move the mouse in camera look mode to rotate about the player and change the forward direction
* Press escape to exit camera look mode, and get your mouse back
* Press the grave/backtick button (`) to toggle the visibility of the X, Y and Z axes (hidden by default)
* Press backspace to reset the model and camera. **You will be asked to re-enter skip size and step size information at the command line.**

## Basic Build Requirements

* [CMake](https://cmake.org) version 3.8 or higher
      * On Windows: Make sure to check option for "add CMake to system path".
      * On Linux: If you need to build from source, [don't forget the `--system-curl` flag](https://github.com/ruslo/hunter/issues/328#issuecomment-198672048)!
* Appropriate C++ build tools for your system:
    * XCode tools on Mac
    * MinGW on Windows
    * Nothing extra to install for Linux
* Recommended IDE for all platforms: CLion (get a free 1-year student license [here](https://www.jetbrains.com/shop/eform/students))

See [Windows MinGW Setup](#markdown-header-windows-mingw-setup) below to see special instructions for installing MinGW.

See [CLion Setup](#markdown-header-clion-setup) below for help importing the project into CLion.

## Running Project in an IDE

Using Clion, you should be able to open the project and press the green play button to run the program.

*Note, dependencies will be downloaded and built from source the first time, so you will need to wait several minutes.*

For more details see [CLion Setup](#markdown-header-clion-project-setup).

## Windows MinGW Setup

1. Download and install MinGW ([click here to install](http://www.mingw.org/download/installer?)).
2. When installation is done hit continue.
3. Select `mingw32-base`, `mingw32-gcc-g++`, and `msys-base`.
4. `Installation -> Apply Changes -> Apply` to download and install the relevant packages.
5. When you see "All changes were applied successfully", hit close and exit the installation manager.

## CLion Setup

*For Windows:* Be sure to follow MinGW setup steps first.

1. If you haven't already, get a free 1-year student license for CLion [here](https://www.jetbrains.com/shop/eform/students).
2. Download and install CLion.
3. In setup, you can accept defaults.
4. Fill out other details as you like - you may want the Markdown support plugin for looking at readme files.

### CLion Project setup
1. On startup select "Open Project", and select the `height-mapper` directory.
2. Once the project is opened, it should automatically build.
4. *Possible Windows error:* if you see a message similar to, `For MinGW to work correctly C:\Program Files\OpenSSH\bin\sh.exe must NOT be in your PATH` go to `File -> Settings -> Appearance & Behavior -> Path Variables` and add `PATH` to the Ignored Variables, hit OK, and try again.
5. Press the green play button to build and run the application.
