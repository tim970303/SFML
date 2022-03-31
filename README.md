# Visual studio setting
Click on project property
C/C++ -> general -> Additional Include directory -> SFML-2.5.1\include
C/C++ -> preprocessor -> preprocessor definition -> SFML_STATIC
Linker -> general -> Additional Library directory -> SFML-2.5.1\lib

Linker -> input ->  
* sfml-graphics-s-d.lib
* sfml-window-s-d.lib
* sfml-system-s-d.lib
* sfml-main-d.lib
* opengl32.lib
* freetype.lib
* winmm.lib
* gdi32.lib