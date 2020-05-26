/*
 * ImageLoader.cpp
 *
 *  Created on: 20 Jun 2012
 *      Author: richard
 */

#include <assert.h>
#include "gfx/ImageLoader.h"
#include "InFile.h"
#include "Common.h"
#include <iostream>

namespace BogDog
{

ImageLoader::ImageLoader()
{
	buffer = NULL;
	bufferSize = 0;

}

ImageLoader::~ImageLoader()
{
	FreeupMemory();
}

void ImageLoader::FreeupMemory()
{
	delete []buffer;
	buffer = NULL;
}

const LoadedImage& ImageLoader::Load(const char* fileName)
{
	loadedImage.image = NULL;

	if( fileName != NULL )
	{
		InFile imageFile;

		if( imageFile.Open(fileName) )
		{
			if(striend(fileName,".tga"))
			{
				LoadTGA((const uint8_t*)imageFile.MapFile(),false);
			}
		}
	}

	return loadedImage;
}

#pragma pack(push,1)
typedef struct TGAHeader
{
	uint8_t  id_length   ;	// number of bytes reserved for image ID (directly after this header)
	uint8_t  map_type    ;	// 0 - no colour map. 1 - includes colour map.
	uint8_t  type        ;	// type of image stored
	uint16_t map_origin  ;	// first index supplied by thtarget_pixel_datae colour map
	uint16_t map_length  ;	// number of entries in the colour map
	uint8_t  map_width   ;	// bits per colour map entry
	uint16_t x_origin    ;	// lower left corner of the image
	uint16_t y_origin    ;	//
	uint16_t width       ;	// width of the image
	uint16_t height      ;	// height of the image
	uint8_t  depth       ;	// # bits per pixel
	uint8_t  alpha_bits  : 4;
	bool  mirrored    : 1;
	bool  flipped     : 1;
	uint8_t  interleaving: 2; // 0 => no interleaving, 1 => way (even/odd) interleaving, 2 => way interleaving (eg: AT&T PC)

	TGAHeader(uint16_t pWidth, uint16_t pHeight)
	{
		id_length = 0;
		map_type = 0;
		type = 2;//TGA_TYPE_TRUECOLOUR;
		map_origin = 0;
		map_length = 0;
		map_width = 0;
		x_origin = 0;
		y_origin = 0;
		width = pWidth;
		height = pHeight;
		depth = 24;
		alpha_bits = 0;
		mirrored = false;
		flipped = true;
		interleaving = 0;
	}

}TGAHeader;
#pragma pack(pop)


void ImageLoader::LoadTGA(const uint8_t *fileData,bool p16Bit_texture)
{
	assert( fileData );

	std::cout << "Loading TGA" << std::endl;

	// read in the TGA header
	const TGAHeader *header = (const TGAHeader*)fileData;

	//ignore image that are too big
	if(header->width > 1024 || header->height > 1024)
		return;

	// set up some local flags to determine the image properties
	uint8_t tga_type = header->type;
	bool rle_compressed = (tga_type & 0x08) != 0;	// RLE compressed image data
	bool huf_compressed = (tga_type & 0x20) != 0;	// Huffman, Delta & RLE compressed
	bool has_alpha = (header->depth == 32) | (header->alpha_bits != 0);

	// not supporting compressed images (yet)
	if(rle_compressed || huf_compressed)
	{
		std::cout << "not supporting compressed images" << std::endl;
		return;
	}

	// determine the target format to load the image as
	loadedImage.textureFormat = TEX_INVALID;
	loadedImage.width = header->width;
	loadedImage.height = header->height;

	std::cout << "Image " << header->width << "x" << header->height << std::endl;

	switch(tga_type&0x07)
	{
	case 0x01:	// indexed image
		std::cout << "indexed image not supported" << std::endl;
		return;	// unsupported (currently)
		break;

	case 0x02:	// RGB image
		// may want to force to 16 bit target
		if(p16Bit_texture)
		{
			loadedImage.textureFormat = has_alpha ? TEX_R5G5B5A1 : TEX_R5G6B5;	break;
		}
		else
		{
			switch(header->depth)
			{
			case 16:	loadedImage.textureFormat = has_alpha ? TEX_R5G5B5A1 : TEX_R5G6B5;	break;
			case 24:	loadedImage.textureFormat = TEX_R8G8B8;	break;
			case 32:	loadedImage.textureFormat = has_alpha ? TEX_R8G8B8A8 : TEX_R8G8B8;	break;
			default:	return;	// unsupported bit depth (currently)
			}
		}
		break;

	case 0x03:	// grey scale image
		// may want to force to 16 bit target
		if(p16Bit_texture)
		{
			loadedImage.textureFormat = has_alpha ? TEX_R5G5B5A1 : TEX_R5G6B5;	break;
		}
		else
		{
			switch(header->depth)
			{
			case 8:	loadedImage.textureFormat = TEX_A8;	break;
			default:	return;	// unsupported bit depth (currently)
			}
		}
		break;

	case 0x00:	// no image data in file
	default:	// unsupported
		std::cout << "no image data in file, not supported" << std::endl;
		return;
	}

	// fill in the target with converted file data
	int source_byte_size = (header->depth>>3);
	int target_byte_size = OpenGLES_2_0::PixelSizeFromFormat(loadedImage.textureFormat);
	int y = -1;
	int y_step = 1;
	int h = header->height;
	if(header->flipped == 0)
	{
		y_step = -1;
		y = header->height;
	}

	std::cout << "target_byte_size " << target_byte_size << std::endl;

	// initialise the target image buffer
	uint8_t* target_pixel_data = getBuffer(header->width, header->height,target_byte_size);


	//Get a pointer to the image data (stored in BGRA byte sequence)
	const uint8_t* file_pixel_data = fileData + header->id_length + header->map_length;

	while( h--)
	{
		y += y_step;
		uint8_t* dest = target_pixel_data + (y * header->width * target_byte_size);
		const uint8_t* source = file_pixel_data;

		//Read next line.
		file_pixel_data += header->width * source_byte_size;

		for( int x = 0 ; x < header->width ; x++ )
		{
			uint8_t component_value[4];

			for(int c = 0 ; c < source_byte_size ; c++ )
			{
				component_value[c] = *source++;
			}

			if(source_byte_size == 1)
			{
				component_value[1] = component_value[0];
				component_value[2] = component_value[0];
				component_value[3] = component_value[0];
			}

			switch(loadedImage.textureFormat)
			{
			case TEX_A8:
				*dest++ = component_value[0];
				break;

			case TEX_R5G6B5:
				*dest++ = ((component_value[1]&0xfc)<<3)|(component_value[0]>>3);
				*dest++ = (component_value[2]&0xf8)|(component_value[1]>>5);
				break;

			case TEX_R5G5B5A1:
				*dest++ = ((component_value[1]&0xf8)<<3)|((component_value[0]&0xfe)>>2)|(component_value[3]&0x01);
				*dest++ = (component_value[2]&0xf8)|(component_value[1]>>5);
				break;

			case TEX_R8G8B8:
				*dest++ = component_value[2];
				*dest++ = component_value[1];
				*dest++ = component_value[0];
				break;

			case TEX_R8G8B8A8:
				*dest++ = component_value[2];
				*dest++ = component_value[1];
				*dest++ = component_value[0];
				*dest++ = component_value[3];
				break;

			default:
				assert(!"LoadTGA failed, unknown destination format");
			}
		}
	}

	loadedImage.image = target_pixel_data;
}

} /* namespace BogDog */
