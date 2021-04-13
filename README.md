# Engine
This is a framework and wrapper for the SFML library that makes creating games easier.

As of right now, there is no documentation. I've been pretty busy, and so I may never get around to adding documentation for it. I periodically update this Engine when I have time. I hope it's useful to someone!

It is required that you use the same compilers that I have. This is because the static libraries included are pre-compiled with Mingw GCC 10.2.0

Download Latest MingW GCC 10.2.0 32bit and 64bit:
https://github.com/brechtsanders/winlibs_mingw/releases/tag/10.2.0-11.0.0-8.0.0-r7

*32bit: winlibs-i686-posix-dwarf-gcc-10.2.0-mingw-w64-8.0.0-r7.7z*

*64bit: winlibs-x86_64-posix-seh-gcc-10.2.0-mingw-w64-8.0.0-r7.7z*

In order to get started, see the Wiki section:

[Get Started With CodeBlocks](https://github.com/jmscreation/Engine/wiki/Getting-Started---Setup-With-CodeBlocks)

[Get Started Without CodeBlocks](https://github.com/jmscreation/Engine/wiki/Getting-Started---MinGW-GCC-Only-(Without-CodeBlocks))

In order to build and modify the Engine Framework library, see the Wiki page:

[Compile The Engine](https://github.com/jmscreation/Engine/wiki/Compile-Engine-Framework)

# Important Linker Settings:

In order to compile your game, you must link the following libraries in this order:

`engine64-s`        Engine Framework (64bit)

`portaudio64-s`     PortAudio (64bit)

`sfml-graphics-s`   SFML

`sfml-window-s`		  SFML

`sfml-system-s`		  SFML

`sfml-network-s`		SFML

`setupapi`			    Used by PortAudio

`winmm`				      Used by SFML

`opengl32`			    Used by SFML

`gdi32`				      Used by SFML

`freetype`			    Used by SFML

`ws2_32`				    Used by SFML

`comdlg32`			    Used by Engine Framework

For 32bit, replace the first 2 linked libraries with:

`engine-s`        Engine Framework (32bit)

`portaudio-s`     PortAudio (32bit)

For older versions:
I still have the 9.2.0 and 7.3.0 compilers that I used to compile previous versions of this library.
To make it easy for anyone having difficulty getting the right compiler, you can download the older versions below.

Here you can download either Mingw GCC 7.3.0 or Mingw GCC 9.2.0 for backwards compatibility:
https://drive.google.com/drive/folders/1WPynZRhfQ73q2lGU1es5IduNChDuE-Of?usp=sharing

(64bit and 32bit compilers are packaged together per MinGW version)

Note:
Future releases will only support Mingw GCC 10.2.0.
See download link above
