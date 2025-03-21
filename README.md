# DioDogHater's multifunction rasterizer
This is a very barebones (and probably unoptimized) attempt to make a rasterizer and 3D engine using basic C.
Now, this project does probably support Windows, but I am unsure, so please be sure to adapt the code if not.
This project's sole purpose is to be able to render 3D primitives in the terminal, giving you the hacker stereotype look.
Well, I guess you can just test it out for yourself and find out what you can do!

# Current state
Right now, this project only supports triangles, because other shapes would be harder to draw. Also, the mesh system
is pretty messy and does waste unecessary ram right now. But I think for now, this project does what it needs to do,
even if it is slow. I think the main performance issue is the fact that we have to iterate through a very very large
number of pixels to test if they are inside a triangle. That alone is already very slow. Might consider using multi-threading,
but that approach is very scary in C, so I dont really want to do it. I could also just turn this into a GPU-Accelerated thing
using OpenGl or something similar, but I just want to keep things simple, and in the terminal.

# Testing and development
I've documented this project very well so please read the comments. Most of the functions are straightforward, and self-descriptive.
But before you delve into this "Engine", just know that you can tweak some things to your liking...
- You can decide wether your project is rendered in the terminal or in SDL by defining either:\
`TRIANGLE_RASTER_TERMINAL` or `TRIANGLE_RASTER_SDL`\
before including `triangleraster.h`.
- You can obviously change the window size by defining `SW` (Screen Width) and `SH` (Screen Height) to the wanted values
before including `triangleraster.h`.
- You can decide wether triangles are rendered when they are winded in counter-clockwise or clockwise order defining either:\
`TRIANGLE_RASTER_CCW` or `TRIANGLE_RASTER_CW`\
before including `triangleraster.h`. Note that it is mandatory to define either one before including the header.
- The `Triangle` struct type has two members:\
`v` -> It is simply the position vector array. The engine uses Z+ view, so please take that into consideration.\
`c` -> It is the color vector array. It just contains the colors to interpolate between vertices during rasterization.

# Requirements
You will need `cmake` and any C compiler to compile this project. Please note that there might be mistakes made for the Windows
port.

# Compiling and building
```
git clone https://github.com/DioDogHater/TriangleRaster
cd TriangleRaster
mkdir build
cmake -S . -B ./build
cmake --build ./build
```
And you're done! The executable will be `./build/raster`.

# Credits
Please credit me if you use this thing, if you want (I dont really give a shit), even though I'm pretty sure nobody will actually stumble upon this.\
\
*This is diodoghater (I dont actually hate dogs)*