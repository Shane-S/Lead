A demonstration application for the libpb library (http://github.com/Shane-S/libpb).

Dependencies (note: these can be installed through package managers in OSX/Linux)
*   [AntTweakBar](http://anttweakbar.sourceforge.net/doc/tools:anttweakbar:download)
*   [SDL2](https://www.libsdl.org/download-2.0.php) (development libraries, e.g. SDL2-devel on OSX/Unix)
*   [SLD2_image](https://www.libsdl.org/projects/SDL_image/) (development libraries)
*   [GLEW](https://sourceforge.net/projects/glew/?source=typ_redirect)
*   [GLM](https://github.com/g-truc/glm/tags)
*   [libpb](http://github.com/Shane-S/libpb)

Build instructions
*   Install CMake (verion 3.0+)
*   On Windows, place each of the dependencies to C:\Program Files\[library name] (and the 32-bit versions in C:\Program Files (x86)\[library name]), e.g. C:\Program Files\SDL2.
*   On OSX/Linux, you shouldn't have to do any other prep if you've installed the dependencies via package manager.
*   Create a build directory and run cmake &lt;Lead download directory&gt; -G"&lt;The generator to use&gt;". For instance, if you created a directory Lead/build and want to generate Unix Makefiles, run cd build; cmake .. -G"Unix Makefils" to create the build system
*   In the same directory, run make (or open your Visual Studio project, etc.) and build the project.