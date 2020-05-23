
SET(CMAKE_SYSTEM_NAME RPi)

SET(CMAKE_CXX_COMPILER ../../broadcom/tools/arm-bcm2708/arm-bcm2708hardfp-linux-gnueabi/arm-bcm2708hardfp-linux-gnueabi/bin/gcc)

SET(CMAKE_FIND_ROOT_PATH ../../broadcom)


# search for programs in the broadcom directories only
SET(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM ONLY)
SET(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
SET(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
