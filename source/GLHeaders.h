
#ifndef __GLHEADERS_H__
#define __GLHEADERS_H__

#ifdef TARGET_GL
	#define GL_GLEXT_PROTOTYPES
	#include "GL/freeglut.h"
	#include "GL/gl.h"
#endif

#ifdef TARGET_GLES
	#include "GLES2/gl2.h"
	#include "EGL/egl.h"
#endif

#ifdef PLATFORM_BCM_HOST
	#include "bcm_host.h"
#endif


#endif //#ifndef __GLHEADERS_H__
