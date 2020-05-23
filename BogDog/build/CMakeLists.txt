
cmake_minimum_required (VERSION 2.8)

#Some defines.
add_definitions(-DTARGET_GL)

#Our include folders
include_directories(../source/ ../source/gfx/ ../source/gl/ ../source/maths/)

#Common files
list(APPEND src ../source/common.cpp ../source/InFile.cpp ../source/View.cpp)

#Code to do with graphics
list(APPEND src ../source/gfx/ImageLoader.cpp ../source/gfx/Mesh.cpp ../source/gfx/ShapeBuilder.cpp)

#GL, the low level rendering code.
list(APPEND src ../source/gl/GLBuffer.cpp ../source/gl/GLShader.cpp ../source/gl/GLShaderColour.cpp ../source/gl/GLShaderColourTex.cpp ../source/gl/OpenGLES20.cpp)

#And ourr maths code.
list(APPEND src ../source/maths/Box.cpp)
list(APPEND src ../source/maths/Frustrum.cpp)
list(APPEND src ../source/maths/Maths.cpp)
list(APPEND src ../source/maths/Matrix.cpp)
list(APPEND src ../source/maths/Plane.cpp)
list(APPEND src ../source/maths/Quaternion.cpp)
list(APPEND src ../source/maths/SinCos.cpp)
list(APPEND src ../source/maths/Vector2.cpp)
list(APPEND src ../source/maths/Vector3.cpp)


project (bogdog)
add_library(bogdog ${src})
