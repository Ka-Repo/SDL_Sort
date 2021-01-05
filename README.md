# SDL_Sort

Visualization of typical sorting algorithms implemented using templates and sdl2.

<br>

## Styleguide

This project follows the C++ Core Guidelines hosted at https ://github.com/isocpp/CppCoreGuidelines.

<br>

#

## Getting started with Visual Studio 2019

<br>

<pre>
   1. Download and extract SDL2 and SDL_ttf from https://www.libsdl.org/projects/SDL_ttf/ and https://www.libsdl.org/download-2.0.php.

</pre>

<br>

<pre>
   2. Add Linker dependencies
    - right click project > Properties > Linker > Additional Dependencies 
    - add SDL2.lib; SDL2main.lib; SDL2_ttf.lib;
</pre>

> Note: use semicolon or new line for separation

<br>

![Alt text](linker.png?raw=true)

<br>
<br>

<pre>
   3. Add Include Directories
    - right click project > Properties > VC++ Directories 
                                       > Include Directories
    - enter $(SolutionDir)\inc
    - enter < path-to-sdl\include >
    - enter < path-to-sdl_ttf\include >
</pre>

<br>

![Alt text](Include.png?raw=true)
<br>
<br>

<pre>
   4. Add Library Directories
    - right click project > Properties > VC++ Directories 
                                       > Library Directories
    - enter $(SolutionDir)\lib
    - enter < path-to-sdl\lib\x64 > or < path-to-sdl_ttf\lib\x86 >
    - enter < path-to-sdl_ttf\lib\x64 > or < path-to-sdl_ttf\lib\x86 >
</pre>

<br>

![Alt text](lib.png?raw=true)

<br>
<br>

> Warning: These settings have to be done for each configuration and plattform your compiling for (see top of project properties)
