/*
 * OpenGLES20.cpp
 *
 *  Created on: 8 Jun 2012
 *
 *  BogDog GLES 2.0 3D Engine for Raspberry Pi
 *	Copyright (C) 2012  Richard e Collins
 *
 *	This program is free software: you can redistribute it and/or modify
 *	it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *	(at your option) any later version.
 *
 *	This program is distributed in the hope that it will be useful,
 *	but WITHOUT ANY WARRANTY; without even the implied warranty of
 *	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *	GNU General Public License for more details.
 *
 * 	You should have received a copy of the GNU General Public License along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <stdio.h>
#include <unistd.h>
#include <sys/time.h>
#include <assert.h>
#include <signal.h>

#include "gl/OpenGLES20.h"
#include "Common.h"
#include "Timer.h"
#include "gfx/ImageLoader.h"

namespace BogDog
{

float channel_to_float_convert[256] =
{
	0.00000000f,0.00392156f,0.00784313f,0.01176470f,0.01568627f,0.01960784f,0.02352941f,0.02745098f,
	0.03137255f,0.03529411f,0.03921568f,0.04313725f,0.04705882f,0.05098039f,0.05490196f,0.05882353f,
	0.06274510f,0.06666667f,0.07058823f,0.07450980f,0.07843137f,0.08235294f,0.08627451f,0.09019608f,
	0.09411764f,0.09803921f,0.10196079f,0.10588235f,0.10980392f,0.11372549f,0.11764706f,0.12156863f,
	0.12549020f,0.12941177f,0.13333334f,0.13725491f,0.14117648f,0.14509805f,0.14901961f,0.15294118f,
	0.15686275f,0.16078432f,0.16470589f,0.16862746f,0.17254902f,0.17647059f,0.18039216f,0.18431373f,
	0.18823530f,0.19215687f,0.19607843f,0.20000000f,0.20392157f,0.20784314f,0.21176471f,0.21568628f,
	0.21960784f,0.22352941f,0.22745098f,0.23137255f,0.23529412f,0.23921569f,0.24313726f,0.24705882f,
	0.25098041f,0.25490198f,0.25882354f,0.26274511f,0.26666668f,0.27058825f,0.27450982f,0.27843139f,
	0.28235295f,0.28627452f,0.29019609f,0.29411766f,0.29803923f,0.30196080f,0.30588236f,0.30980393f,
	0.31372550f,0.31764707f,0.32156864f,0.32549021f,0.32941177f,0.33333334f,0.33725491f,0.34117648f,
	0.34509805f,0.34901962f,0.35294119f,0.35686275f,0.36078432f,0.36470589f,0.36862746f,0.37254903f,
	0.37647060f,0.38039216f,0.38431373f,0.38823530f,0.39215687f,0.39607844f,0.40000001f,0.40392157f,
	0.40784314f,0.41176471f,0.41568628f,0.41960785f,0.42352942f,0.42745098f,0.43137255f,0.43529412f,
	0.43921569f,0.44313726f,0.44705883f,0.45098040f,0.45490196f,0.45882353f,0.46274510f,0.46666667f,
	0.47058824f,0.47450981f,0.47843137f,0.48235294f,0.48627451f,0.49019608f,0.49411765f,0.49803922f,
	0.50196081f,0.50588238f,0.50980395f,0.51372552f,0.51764709f,0.52156866f,0.52549022f,0.52941179f,
	0.53333336f,0.53725493f,0.54117650f,0.54509807f,0.54901963f,0.55294120f,0.55686277f,0.56078434f,
	0.56470591f,0.56862748f,0.57254905f,0.57647061f,0.58039218f,0.58431375f,0.58823532f,0.59215689f,
	0.59607846f,0.60000002f,0.60392159f,0.60784316f,0.61176473f,0.61568630f,0.61960787f,0.62352943f,
	0.62745100f,0.63137257f,0.63529414f,0.63921571f,0.64313728f,0.64705884f,0.65098041f,0.65490198f,
	0.65882355f,0.66274512f,0.66666669f,0.67058825f,0.67450982f,0.67843139f,0.68235296f,0.68627453f,
	0.69019610f,0.69411767f,0.69803923f,0.70196080f,0.70588237f,0.70980394f,0.71372551f,0.71764708f,
	0.72156864f,0.72549021f,0.72941178f,0.73333335f,0.73725492f,0.74117649f,0.74509805f,0.74901962f,
	0.75294119f,0.75686276f,0.76078433f,0.76470590f,0.76862746f,0.77254903f,0.77647060f,0.78039217f,
	0.78431374f,0.78823531f,0.79215688f,0.79607844f,0.80000001f,0.80392158f,0.80784315f,0.81176472f,
	0.81568629f,0.81960785f,0.82352942f,0.82745099f,0.83137256f,0.83529413f,0.83921570f,0.84313726f,
	0.84705883f,0.85098040f,0.85490197f,0.85882354f,0.86274511f,0.86666667f,0.87058824f,0.87450981f,
	0.87843138f,0.88235295f,0.88627452f,0.89019608f,0.89411765f,0.89803922f,0.90196079f,0.90588236f,
	0.90980393f,0.91372550f,0.91764706f,0.92156863f,0.92549020f,0.92941177f,0.93333334f,0.93725491f,
	0.94117647f,0.94509804f,0.94901961f,0.95294118f,0.95686275f,0.96078432f,0.96470588f,0.96862745f,
	0.97254902f,0.97647059f,0.98039216f,0.98431373f,0.98823529f,0.99215686f,0.99607843f,1.00000000f,
};

static bool applicationRunning = false;

static void ExitApplication()
{
	printf("Recived app close message\n");
	applicationRunning = false;
}

#ifdef PLATFORM_RPI_VC4_GLES
static void RPI_Exit(int)
{
	ExitApplication();
}
#endif

OpenGLES_2_0::OpenGLES_2_0()
{
#ifdef PLATFORM_RPI_VC4_GLES
	struct sigaction act;
	act.sa_handler = RPI_Exit;
	sigemptyset(&act.sa_mask);
	act.sa_flags = 0;
	sigaction(SIGINT,&act,0);
#endif

	printf("BogDog GLES 2.0 3D Engine for Raspberry Pi Copyright (C) 2012 Richard Eric Collins\n");
	printf("This library comes with ABSOLUTELY NO WARRANTY.\n");
	printf("This is free software, and you are welcome to redistribute it under certain conditions.\n");
	printf("see <http://www.gnu.org/licenses/>\n");

	printf("\n***** Some type sizes I use *****\n");
	printf("sizeof(int) = %d\n",(int)sizeof(int));
	printf("sizeof(long) = %d\n",(int)sizeof(long));
	printf("sizeof(long int) = %d\n",(int)sizeof(long int));
	printf("sizeof(long long) = %d\n",(int)sizeof(long long));
	printf("sizeof(uint64_t) = %d\n",(int)sizeof(uint64_t));
	printf("sizeof(float) = %d\n",(int)sizeof(float));
	printf("sizeof(size_t) = %d\n",(int)sizeof(size_t));
	printf("sizeof(timeval) = %d\n",(int)sizeof(timeval));
	printf("sizeof(__time_t) = %d\n",(int)sizeof(__time_t));
	printf("sizeof(__suseconds_t) = %d\n",(int)sizeof(__suseconds_t));
	printf("sizeof(Angle) = %d\n",(int)sizeof(Angle));
	printf("*********************************\n\n");
}

OpenGLES_2_0::~OpenGLES_2_0()
{

}

bool OpenGLES_2_0::ApplicationRunning()
{
	return applicationRunning;
}

bool OpenGLES_2_0::Create(bool syncWithDisplay)
{
#ifdef PLATFORM_RPI_VC4_GLES
	bcm_host_init();
#endif

	if( !OpenGLES(syncWithDisplay) )
		return false;

	CHECK_OGL_ERRORS();

	printf("Display at %dx%d\n",m_info.width,m_info.height);

	applicationRunning = true;

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	glDepthMask(true);

	glEnable(GL_CULL_FACE);
	glFrontFace(GL_CW);
	glCullFace(GL_BACK);

	glPixelStorei(GL_UNPACK_ALIGNMENT,1);
	SetBlendMode(BLENDMODE_OFF);

#ifdef TARGET_GLES
	glDepthRangef(0.0f,1.0f);
#endif//#ifdef TARGET_GLES

	CHECK_OGL_ERRORS();


	return true;
}

void OpenGLES_2_0::SetBlendMode(BLENDMODE mode)
{
	switch(mode)
	{
	case BLENDMODE_OFF:
		glDisable(GL_BLEND);
		break;

	case BLENDMODE_NORMAL:
		glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
		glBlendEquation(GL_FUNC_ADD);
		glEnable(GL_BLEND);
		break;

	case BLENDMODE_ADDITIVE:
		glBlendFunc(GL_SRC_ALPHA,GL_ONE);
		glBlendEquation(GL_FUNC_ADD);
		glEnable(GL_BLEND);
		break;

	case BLENDMODE_SUBTRACTIVE:
		glBlendFunc(GL_SRC_ALPHA,GL_ONE);
		glBlendEquation(GL_FUNC_SUBTRACT);
		glEnable(GL_BLEND);
		break;
	}
	CHECK_OGL_ERRORS();
}

void OpenGLES_2_0::Update()
{
#ifdef TARGET_GLES
	eglSwapBuffers(m_display,m_surface);
#endif //#define TARGET_GLES

#ifdef TARGET_GL
	glutSwapBuffers();
	glutMainLoopEvent();
#endif //#define TARGET_GL

	CHECK_OGL_ERRORS();
}

void OpenGLES_2_0::Clear(uint32_t pRed, uint32_t pGreen, uint32_t pBlue)
{
	glClearColor(
			channel_to_float_convert[pRed&255],
			channel_to_float_convert[pGreen&255],
			channel_to_float_convert[pBlue&255],
			1.0f);
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
	CHECK_OGL_ERRORS();
}

void OpenGLES_2_0::Clear(uint32_t pColour)
{
	return Clear(GetRed(pColour),GetGreen(pColour),GetBlue(pColour));
}


void OpenGLES_2_0::ClearZ()
{
	glClear(GL_DEPTH_BUFFER_BIT);
	CHECK_OGL_ERRORS();
}

GLuint OpenGLES_2_0::CreateTexture(TextureFormat textureFormat,int width,int height,const GLvoid* pixels,bool mipMap,bool filtered,bool uvClamp)
{
	assert(pixels != NULL );

	GLint format = GL_RGB;
	GLint gl_format = GL_UNSIGNED_BYTE;
	switch( textureFormat )
	{
	case TEX_R4G4B4A4:
		format = GL_RGBA;
		gl_format = GL_UNSIGNED_SHORT_4_4_4_4;
		break;

	case TEX_R5G6B5:
		gl_format = GL_UNSIGNED_SHORT_5_6_5;
		break;

	case TEX_R5G5B5A1:	//16 bit
		format = GL_RGBA;
		gl_format = GL_UNSIGNED_SHORT_5_5_5_1;
		break;

	case TEX_R8G8B8:		//24 bit
		format = GL_RGB;
		break;

	case TEX_R8G8B8A8:	//32 bit
		format = GL_RGBA;
		break;

	case TEX_A8:	//8 bit
		format = GL_ALPHA;
		break;

	default:
		assert(!"Unsupported pixel format");
		return 0;
	}

	GLuint tex;
	glGenTextures(1,&tex);
	CHECK_OGL_ERRORS();
	assert(tex);

	glBindTexture(GL_TEXTURE_2D,tex);
	CHECK_OGL_ERRORS();

	glTexImage2D(
		GL_TEXTURE_2D,
		0,
		format,
		width,
		height,
		0,
		format,
		gl_format,
		pixels);

	CHECK_OGL_ERRORS();

	//Unlike GLES 1.1 this is called after texture creation, in GLES 1.1 you say that you want glTexImage2D to make the mips.
	if( mipMap )
	{
		glGenerateMipmap(GL_TEXTURE_2D);
		CHECK_OGL_ERRORS();
		if( filtered )
		{
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		}
		else
		{
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		}
	}
	else
	{
		if( filtered )
		{
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		}
		else
		{
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		}
	}

	CHECK_OGL_ERRORS();

	if( uvClamp )
	{
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	}
	else
	{
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	}

	glBindTexture(GL_TEXTURE_2D,0);//Because we had to change it to setup the texture! Stupid GL!
	CHECK_OGL_ERRORS();

	return tex;
}

GLuint OpenGLES_2_0::CreateTexture(const LoadedImage& image, bool mipMap,bool filtered,bool uvClamp)
{
	if( image.image == NULL )
	{
		return 0;
	}
	return CreateTexture(image.textureFormat,image.width,image.height,image.image,mipMap,filtered,uvClamp);
}

int OpenGLES_2_0::GetGLTypeSize(int type)
{
	switch (type)
	{
	case GL_FLOAT:
	case GL_INT:
	case GL_UNSIGNED_INT:
		return 4;

	case GL_SHORT:
	case GL_UNSIGNED_SHORT:
		return 2;

	case GL_BYTE:
	case GL_UNSIGNED_BYTE:
		return 1;
	}
	return 0;
}

void OpenGLES_2_0::ReadOGLErrors(const char *pSource_file_name,int pLine_number)
{
	int gl_error_code = glGetError();
	if( gl_error_code == GL_NO_ERROR )
	{
		return;
	}

//	GLOBAL_ASSERT( gl_error_code == GL_NO_ERROR );

	printf("\n**********************\nline %d file %s\n",pLine_number,pSource_file_name);
	while(gl_error_code != GL_NO_ERROR)
	{
		printf("GL error[%d]: :",gl_error_code);
		switch(gl_error_code)
		{
		default:
			printf("Unknown OGL error code\n");
			break;

		case GL_INVALID_ENUM:
			printf("An unacceptable value is specified for an enumerated argument. The offending command is ignored, having no side effect other than to set the error flag.\n");
			break;

		case GL_INVALID_VALUE:
			printf("A numeric argument is out of range. The offending command is ignored, having no side effect other than to set the error flag.\n");
			break;

		case GL_INVALID_OPERATION:
			printf("The specified operation is not allowed in the current state. The offending command is ignored, having no side effect other than to set the error flag.\n");
			break;

		case GL_OUT_OF_MEMORY:
			printf("There is not enough memory left to execute the command. The state of the GL is undefined, except for the state of the error flags, after this error is recorded.\n");
			break;
		}
		//Get next error.
		int last = gl_error_code;
		gl_error_code = glGetError();
		if( last == gl_error_code )
			break;
	}

	printf("**********************\n");
}

bool OpenGLES_2_0::OpenGLES(bool syncWithDisplay)
{
#ifdef TARGET_GLES

	m_display = eglGetDisplay(EGL_DEFAULT_DISPLAY);

	if( !m_display )
	{
		printf("Error: Couldn\'t open the EGL default display\n");
		return false;
	}

	//Now we have a display lets initialize it.
	if( !eglInitialize(m_display, &m_major_version, &m_minor_version) )
	{
		printf("Error: eglInitialize() failed\n");
		return false;
	}

	printf("m_major_version(%d) m_minor_version(%d)\n",m_major_version, m_minor_version);

	if( !GetGLConfig() )
	{
		return false;
	}

	eglBindAPI(EGL_OPENGL_ES_API);


	//We have our display and have chosen the config so now we are ready to create the rendering context.
	EGLint ai32ContextAttribs[] = { EGL_CONTEXT_CLIENT_VERSION, 2, EGL_NONE };
	m_context = eglCreateContext(m_display,m_config,EGL_NO_CONTEXT,ai32ContextAttribs);
	if( !m_context )
	{
		printf("Error: Failed to get a rendering context.\n");
		return false;
	}

#ifdef PLATFORM_RPI_VC4_GLES
	VC_RECT_T dst_rect;
	VC_RECT_T src_rect;
	VC_DISPMANX_ALPHA_T alpha;

	// create an EGL window surface
	int32_t success = graphics_get_display_size(0 /* LCD */,(uint32_t*)&m_info.width,(uint32_t*)&m_info.height);
	if( success < 0 )
	{
		printf("graphics_get_display_size failed, %d\n",success);
		return false;
	}

	dst_rect.x = 0;
	dst_rect.y = 0;
	dst_rect.width = m_info.width;
	dst_rect.height = m_info.height;

	src_rect.x = 0;
	src_rect.y = 0;
	src_rect.width = m_info.width << 16;
	src_rect.height = m_info.height << 16;

	alpha.opacity = 255;
	alpha.flags = DISPMANX_FLAGS_ALPHA_FIXED_ALL_PIXELS;//DISPMANX_FLAGS_ALPHA_FIXED_NON_ZERO;

	DISPMANX_DISPLAY_HANDLE_T dispman_display = vc_dispmanx_display_open( 0 /* LCD */);
	DISPMANX_UPDATE_HANDLE_T dispman_update = vc_dispmanx_update_start( 0 );

	DISPMANX_ELEMENT_HANDLE_T dispman_element = vc_dispmanx_element_add ( dispman_update, dispman_display,
	   0/*layer*/, &dst_rect, 0/*src*/,
	   &src_rect, DISPMANX_PROTECTION_NONE, &alpha /*alpha*/, 0/*clamp*/, DISPMANX_NO_ROTATE/*transform*/);

	m_native_window.element = dispman_element;
	m_native_window.width = m_info.width;
	m_native_window.height = m_info.height;
	vc_dispmanx_update_submit_sync( dispman_update );
#endif //PLATFORM_RPI_VC4_GLES

#ifdef PLATFORM_MESA
	m_native_window = 0;
#endif

	m_surface = eglCreateWindowSurface(m_display,m_config,m_native_window,0);

	if( m_surface == EGL_NO_SURFACE )
	{
		printf("Error: Failed to create display surfaces\n");
		return false;
	}


	eglMakeCurrent( m_display, m_surface, m_surface, m_context );

	eglQuerySurface(m_display, m_surface,EGL_WIDTH,  &m_info.width);
	eglQuerySurface(m_display, m_surface,EGL_HEIGHT, &m_info.height);

//	glColorMask(EGL_TRUE,EGL_TRUE,EGL_TRUE,EGL_FALSE);

	if( syncWithDisplay )
	{
		eglSwapInterval(m_display,1);
	}
	else
	{
		eglSwapInterval(m_display,0);
	}

	m_info.display_aspect = (float)m_info.width / (float)m_info.height;

	return true;
#endif //#ifdef TARGET_GLES

#ifdef TARGET_GL
	m_info.width = 1920 / 2;
	m_info.height = 1080 / 2;
	int n = 0;
	const char* args[] = {"",NULL};
	glutInit(&n,(char**)args);
	glutInitDisplayMode (GLUT_SINGLE | GLUT_RGBA);
	glutInitWindowSize(m_info.width,m_info.height);
	glutInitWindowPosition(0,0);
	glutCreateWindow("simple");
	glutMainLoopEvent();
	glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE,GLUT_ACTION_CONTINUE_EXECUTION);

	glutCloseFunc(ExitApplication);

	return true;
#endif //#ifdef TARGET_GL
}

bool OpenGLES_2_0::GetGLConfig()
{
#ifdef TARGET_GLES

	int depths_32_to_16[3] = {32,24,16};

	m_config = (EGLConfig)-1;
	for( int c = 0 ; c < 3 ; c++ )
	{
		const EGLint attrib_list[] =
		{
			EGL_RED_SIZE,			8,
			EGL_GREEN_SIZE,			8,
			EGL_BLUE_SIZE,			8,
			EGL_ALPHA_SIZE,			8,
			EGL_DEPTH_SIZE,			depths_32_to_16[c],
			EGL_RENDERABLE_TYPE,	EGL_OPENGL_ES2_BIT,
			EGL_NONE,				EGL_NONE
		};

		EGLint numConfigs;
		if( !eglChooseConfig(m_display,attrib_list,&m_config,1, &numConfigs) )
		{
			printf("Error: eglGetConfigs() failed\n");
			return false;
		}

		if( numConfigs > 0 )
		{
			EGLint bufSize,r,g,b,a,z,s = 0;

			eglGetConfigAttrib(m_display,m_config,EGL_BUFFER_SIZE,&bufSize);

			eglGetConfigAttrib(m_display,m_config,EGL_RED_SIZE,&r);
			eglGetConfigAttrib(m_display,m_config,EGL_GREEN_SIZE,&g);
			eglGetConfigAttrib(m_display,m_config,EGL_BLUE_SIZE,&b);
			eglGetConfigAttrib(m_display,m_config,EGL_ALPHA_SIZE,&a);

			eglGetConfigAttrib(m_display,m_config,EGL_DEPTH_SIZE,&z);
			eglGetConfigAttrib(m_display,m_config,EGL_STENCIL_SIZE,&s);

			printf("%d Configs found:\n\tFrame buffer(%d) RGBA(%d %d %d %d)\n\tZBuffer(%d) Z(%d) S(%d)\n\n",numConfigs,bufSize,r,g,b,a,z+s,z,s);
			return true;
		}
	}

	printf("Error: No matching configs\n");
	return false;
#endif //#ifdef TARGET_GLES

#ifdef TARGET_GL
	return true;
#endif
}



} /* namespace BogDog */

