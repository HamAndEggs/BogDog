/*
 * ImageLoader.h
 *
 *  Created on: 20 Jun 2012
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

#ifndef IMAGELOADER_H_
#define IMAGELOADER_H_

#include <stddef.h>
#include <stdint.h>

#include "gl/OpenGLES20.h"


namespace BogDog
{

struct InFile;

/*
 * This is the loaded image returned by Load. This is only valid until the next call to Load.
 * Do not free it or allocate them your self. This is managed by the LoadedImage class.
 */
struct LoadedImage
{
	int width,height;
	TextureFormat textureFormat;

	/*
	 * The loaded image data, null if the image could not be loaded.
	 */
	const void* image;
};

/*
 * An object that can be used to load images. We have it in an object so that we can have a buffer that is reused
 * instead of allocating one over and over again. Memory operations are costly. GL does not give us a way
 * to load textures directly into video ram.
 * Because of this buffer the class is not thread safe. But then you should not be loading
 * on two different threads, that is slow.
 */
struct ImageLoader
{
	ImageLoader();
	virtual ~ImageLoader();

	/*
	 * Free's up the memory allocated for loading textures.
	 * Best called after all textures have been loaded.
	 */
	void FreeupMemory();

	const LoadedImage& Load(const char* fileName);

private:
	uint8_t* buffer;
	int bufferSize;

	/*
	 * For returning back to the caller.
	 */
	LoadedImage loadedImage;

	/*
	 * Gets the work buffer for the loaded texture to be written to.
	 * This is returned as the image data when a texture is loaded ok.
	 * As a result a texture must be used before then next one is loaded.
	 */
	uint8_t* getBuffer(int width,int height,int depth)
	{
		int newSize = width * height * depth;
		if( bufferSize < newSize )
		{
			bufferSize = newSize;
			delete []buffer;
			buffer = new uint8_t[newSize];
		}
		return buffer;
	}

	void LoadTGA(const uint8_t *fileData,bool p16Bit_texture);
};

} /* namespace BogDog */
#endif /* IMAGELOADER_H_ */
