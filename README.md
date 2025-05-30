# DioDogHater's multifunction Rasterizer (*DIO_RASTER*)
This is a very barebones (and probably unoptimized) attempt to make a rasterizer and 3D engine using plain C, in the terminal.
This project's sole purpose is to be able to render 3D shapes in the terminal, giving you the hacker stereotype look.
Well, I guess you can just test it out for yourself and find out what you can do!

# Current state
Right now, this project only supports triangles, because other shapes would be harder to draw. Also, the mesh system
is pretty messy and does waste unecessary ram right now. But I think for now, this project does what it needs to do,
even if it is slow. I think the main performance issue is the fact that we have to iterate through a very very large
number of pixels to test if they are inside a triangle. I used multi-threading to make it a little bit faster, but it
still stutters and slows down sometimes. Honestly, I think it actually worsens the speed of my renderer.
I could also just turn this into a GPU-Accelerated thing using OpenGl or something similar, but I just want to keep
things simple, and in the terminal.

# Testing and development
I've documented this project *decently* so please read the comments. Most of the functions are straightforward, and self-descriptive.
But before you delve into this "Engine", just know that you can tweak some things to your liking...
- You can decide wether your project is rendered in the terminal or inside a SDL window by defining either:\
`DIO_RASTER_TERMINAL` or `DIO_RASTER_SDL`\
before including `dioraster.h`.
- You can obviously change the window size by defining `SW` (Screen Width) and `SH` (Screen Height) to the wanted values
before including `dioraster.h`.
- You can decide wether triangles are rendered when they are winded in counter-clockwise or clockwise order defining either:\
`DIO_RASTER_CCW` or `DIO_RASTER_CW`\
before including `dioraster.h`. Note that it is mandatory to define either one before including the header.
- You can decide in "*Terminal*" rendering mode wether you want to use black and white gradients or full color (only supported on certain terminals).
To enable the full color mode, define: `DIO_RASTER_FULL_COLOR`. Otherwise, the rendering will be done in black and white gradients using the
"density" of ASCII characters.
- You can choose the intensity of the ambient light and the diffuse lighting with the macros\
`DIO_RASTER_AMBIENT` and `DIO_RASTER_DIFFUSE` by defining them to your desired value before including
`dioraster.h`.

# Requirements
This is a Linux / Unix only project. You might have to tweak things to get it to work with MacOS.\
You will need the `cmake` build platform and any C compiler to compile this project.\
IF YOU DO NOT WANT TO USE SDL2 AT ALL, PLEASE REMOVE THE FOLLOWING LINE IN `CMakeLists.txt`:\
`target_link_libaries(raster SDL2)`\
If you desire to use SDL too, you will need to have the libary installed and link it properly. I use the system-wide installed
version of SDL2, but for other installations of SDL2, you need to set it up yourself in `CMakeLists.txt`.

# Using Wavefront (.obj) meshes
If you want to use Wavefront meshes, please note that for now, materials and textures are not supported. Also, since the .obj file parser is still barebones for now, you will maybe get weird errors. In that case, you can remove all lines that don't start with 'v' or 'f'.
Also, please triangulate your mesh's faces before using them (transform the quads in the mesh into triangles).
For now, a loaded model can only have a single uniform color, but I might add textures later on.
For the model to be accurate, save your mesh with Z+ as the forward axis, Y as the up axis, no edges, no materials and remove all unecessary elements in the scene such as cameras, lights, etc.

# Compiling and building
```
git clone https://github.com/DioDogHater/DioRaster
cd DioRaster
mkdir build
cmake -S . -B ./build
cmake --build ./build
```
Then after that first build, you can just use this line:\
`cmake --build ./build`\
To compile everytime you want to test out your code!\
And you're done! The executable will be `./build/raster`.

# The cube example
![A cube rotating in 3D, rendered in ASCII](https://github.com/DioDogHater/DioDogHater/blob/fde1610ef43855ae6795829926043d9244c518e5/triangleraster_cube.gif "The Cube.")\
Then... Behold: Lighting!\
![A shaded cube rotating in 3D, rendered in ASCII](https://github.com/DioDogHater/DioDogHater/blob/ebe9eb04076de24d05cd04276cd687b588c0048e/triangleraster_shadedcube.gif "The Shaded Cube.")

# The car example
Now this is the coolest thing I've been able to do so far!\
Golden scar? No, but golden car. Vroom vroom.\
![A car rotating in 3D, rendered using hashtags in a gold color](https://github.com/DioDogHater/DioDogHater/blob/eb32255b533146afe500c420d3f5e5576db2fa4a/triangleraster_car.gif "The Golden Car.")

# My attempt at copying Donut C
Cool *spinning* donut.\
![Donut rotating in 3D using ASCII shading](https://github.com/DioDogHater/DioDogHater/blob/15e2a362b9d393115e25063826c47a8f35d91926/triangleraster_donut.gif "The Donut.")

# Cool multi-mesh example
Car in the back while the monkey spins uncontrollably.\
![Car and monkey (suzanne) spinning in 3D, in grayscale](https://github.com/DioDogHater/DioDogHater/blob/f845722920819ad3b338ae85dd3b26889b274f81/triangleraster_multmesh_grayscale.gif "The Multimesh Example.")\
![Car and monkey (suzanne) spinning in 3D, in color](https://github.com/DioDogHater/DioDogHater/blob/f845722920819ad3b338ae85dd3b26889b274f81/triangleraster_multimesh_color.gif "The Multimesh Example.")

# Credits
Please credit me if you use this thing, if you want (I dont really give a shit), even though I'm pretty sure nobody will actually stumble upon this.\
\
*This is diodoghater (I dont actually hate dogs)*
