/*
 * GLBuffer.cpp
 *
 *  Created on: 9 Jun 2012
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

#include "./gl/GLBuffer.h"
#include "./gl/OpenGLES20.h"
namespace BogDog
{

GLBuffer::GLBuffer(int pCount,int pElementCount,int pDataType,int pTarget,int pUsage)
{
	count = pCount;
	dataType = pDataType;
	elementCount = pElementCount;
	target = pTarget;
	usage = pUsage;
	elementSize = OpenGLES_2_0::GetGLTypeSize(pDataType) * pElementCount;
	memSize = pCount * elementSize;

//	glGenBuffers(1,&buffer);

	//Get the buffer to allocate it's data.
/*	glBindBuffer(target,buffer);
	glBufferData(target, memSize, NULL, usage);*/

	bufferEmulation = malloc(memSize);
}

GLBuffer::~GLBuffer()
{

}

} /* namespace BogDog */
