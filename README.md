# Modulus Woods

You're a brave Jack-O-Lantern exploring Modulus Woods, a procedurally generated world of curious shrubbery.

*Screenshots: Coming Soon*

Developed with C++ and OpenGL for macOS, Windows and Linux.

## Features

* Explore an endless world
* Save and load seeds which will regenerate the world as you last saw it

### Controls

* Use WASD or the arrow keys to move the player forward, back, left or right.
* Click the window with the left mouse button to capture the mouse and enter camera look mode.
* Move the mouse in camera look mode to rotate about the player and change the forward direction.
* Press escape to exit camera look mode, and get your mouse back
* Scroll up to narrow the viewer perspective (zoom in) and scroll down to zoom out.
* Press the `0` key at any time to print a seed value to the console. You can enter this later to generate a world at the same starting position.
* Press the grave/backtick button (`) to toggle the visibility of the X, Y and Z axes (hidden by default)

## Building

See [Building and running the project](docs/building.md).

## Acknowledgements

### Dependencies

Beyond the C++ standard library this application relies on:
* [OpenGL](https://www.opengl.org/)
* [GLFW](http://www.glfw.org/)
* [GLEW](http://glew.sourceforge.net/)
* [GLM](https://glm.g-truc.net/0.9.8/index.html)
* [tinyobjloader](https://github.com/syoyo/tinyobjloader)
* [stb image](https://github.com/nothings/stb)

(Following standard setup procedure in a CMake-compatible IDE should fetch all these dependencies for you, so you shouldn't need to download them separately.)

### Other help

* Starter skeleton code adapted from [LearnOpenGL.com](https://learnopengl.com/) and COMP 371 at Concordia University
* Cross platform CMake setup created using [Hunter](https://github.com/ruslo/hunter)

