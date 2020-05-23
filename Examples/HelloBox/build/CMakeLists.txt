cmake_minimum_required (VERSION 2.6)

#Some defines.
add_definitions(-DTARGET_GL)

include_directories(../../BogDog/source)
link_directories(../../BogDog/build)

project (HelloBox)
add_executable(HelloBox ../main.cpp)
add_dependencies(HelloBox ../../BogDog/build)
target_link_libraries(HelloBox bogdog glut)
