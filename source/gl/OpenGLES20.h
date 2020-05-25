/*
 * OpenGLES20.h
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

#ifndef OPENGLES20_H_
#define OPENGLES20_H_

#include "GLHeaders.h"

//#ifdef _DEBUG
//	#define CHECK_OGL_ERRORS()	BogDog::OpenGLES_2_0::ReadOGLErrors(__FILE__,__LINE__)
//#else
	#define CHECK_OGL_ERRORS()
//#endif

namespace BogDog
{

/// @cond Doxygen should ignore this
extern float channel_to_float_convert[256];
/// @endcond

/*!
	Extract the value of the red component from an RMColour.

	@param	colour	Colour in RMColour format.
	@return			Intensity of the red component, in range 0 to 255.
*/
inline uint32_t GetRed(uint32_t colour)
{
	return (uint32_t)((colour&0xff));
}

/*!
	Extract the value of the green component from an RMColour.

	@param	colour	Colour in RMColour format.
	@return			Intensity of the green component, in range 0 to 255.
*/
inline uint32_t GetGreen(uint32_t colour)
{
	return (uint32_t)((colour&0x0000ff00)>>8);
}

/*!
	Extract the value of the blue component from an RMColour.

	@param	colour	Colour in RMColour format.
	@return			Intensity of the blue component, in range 0 to 255.
*/
inline uint32_t GetBlue(uint32_t colour)
{
	return (uint32_t)((colour&0x00ff0000)>>16);
}

/*!
	Extract the value of the alpha component from an RMColour.

	@param	colour	Colour in RMColour format.
	@return			Intensity of the alpha component, in range 0 to 255.
*/
inline uint32_t GetAlpha(uint32_t colour)
{
	return (uint32_t)((colour&0xff000000)>>24);
}



enum BLENDMODE
{
	BLENDMODE_OFF,
	BLENDMODE_NORMAL,
	BLENDMODE_ADDITIVE,
	BLENDMODE_SUBTRACTIVE
};

/*
 * Some formats I use in my texture allocation function.
 * There is nothing stopping you doing your own texture allocation.
 * My function just makes it simpler for some common formats.
 */
typedef enum
{
	TEX_A8,			// 8 bit
	TEX_R4G4B4A4,	// 16 bit
	TEX_R5G6B5,		// 16 bit
	TEX_R5G5B5A1,	// 16 bit
	TEX_R8G8B8,		// 24 bit
	TEX_R8G8B8A8,	// 32 bit
	TEX_INVALID = 0x7fffffff,
}TextureFormat;

struct LoadedImage;

struct OpenGLES_2_0
{
	OpenGLES_2_0();
	virtual ~OpenGLES_2_0();

	/*
	 * Returns true while the application is running.
	 * Done like this as the desktop and RPi exit differently.
	 * Allows the application to clean up.
	 */
	static bool ApplicationRunning();

	bool Create(bool syncWithDisplay);

	void SetBlendMode(BLENDMODE mode);

	void Update();
	void Clear(uint32_t pRed, uint32_t pGreen, uint32_t pBlue);
	void Clear(uint32_t pColour);
	void ClearZ();

	float GetAspectRatio(){return (float)m_info.width / (float)m_info.height;}

	GLuint CreateTexture(TextureFormat textureFormat,int width,int height,const GLvoid* pixels,bool mipMap = true,bool filtered = true,bool uvClamp = false);

	GLuint CreateTexture(const LoadedImage& image,bool mipMap = true,bool filtered = true,bool uvClamp = false);

	/*!
	 * Get the size of the GL prim type.
	 *
	 * @param type The prim type.
	 * @return The size of the type in bytes.
	 */
	static int GetGLTypeSize(int type);


	static void ReadOGLErrors(const char *pSource_file_name,int pLine_number);

	static int PixelSizeFromFormat(TextureFormat format)
	{
		switch(format)
		{
		case TEX_A8:
			return 1;

		case TEX_R4G4B4A4:
		case TEX_R5G6B5:
		case TEX_R5G5B5A1:
			return 2;

		case TEX_R8G8B8:
			return 3;

		case TEX_R8G8B8A8:
			return 4;

		case TEX_INVALID:
			return 0;
		}
		return 0;
	}


private:
#ifdef TARGET_GLES
	EGLDisplay m_display;						//!<GL display
	EGLSurface m_surface;						//!<GL rendering surface
	EGLContext m_context;						//!<GL rendering context
	EGLConfig m_config;							//!<Configuration of the display.
    EGLint m_major_version,m_minor_version;		//!<Version number of OpenGLES we are running on.
#endif

#ifdef PLATFORM_RPI_VC4_GLES
    EGL_DISPMANX_WINDOW_T m_native_window;			//!<The RPi window object needed to create the render surface.
#endif

#ifdef PLATFORM_MESA // sudo apt install libgles2-mesa-dev
	EGLNativeWindowType m_native_window;
#endif

	struct
	{
    	int width,height;
		float display_aspect;
	}m_info;

	bool OpenGLES(bool syncWithDisplay);
	bool GetGLConfig();

};

} /* namespace BogDog */
#endif /* OPENGLES20_H_ */
