/*
 * GLBuffer.h
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

#ifndef GLBUFFER_H_
#define GLBUFFER_H_

#include <memory.h>
#include "GLHeaders.h"
#include "OpenGLES20.h"
#include "Common.h"
#include "./maths/Vector3.h"

namespace BogDog
{

struct GLBuffer
{
	/**
	 * Creates a buffer.
	 * I have created some extensions to this class for common used combinations such as x,y,z, colours, UV and indices.
	 * @param count The number of 'vertices' or element sets in the buffer. For example number of xyz sets.
	 * @param elementCount The number of elements, for example for xyz it will be 3.
	 * @param dataType The GL data type, for example GL_FLOAT.
	 * @param target The target, GL_ARRAY_BUFFER or GL_ELEMENT_ARRAY_BUFFER.
	 * @param usage
	 */
	GLBuffer(int count,int elementCount,int dataType,int target,int usage);

	~GLBuffer();

	/**
	 * @return The number of element groups, think of it as vertex count where one vertex could be three floats for XYZ.
	 */
	int getCount()
	{
		return count;
	}

	/**
	 * Expects that the data object passed will fill all of the object.
	 * @param data
	 */
	void setData(void* data)
	{
/*		glBindBuffer(target,buffer);
        CHECK_OGL_ERRORS();
		glBufferData(target, memSize, data, usage);
        CHECK_OGL_ERRORS();*/
		memcpy(bufferEmulation,data,memSize);
	}

	/**
	 * Assumes you are setting the whole element, that is for an xyz buffer data will be 4 * 3 bytes in size. float == 4, xyz == 3.
	 * I think I could improve this with native code function, but for now this should be called very little once the models are created.
	 * Pick you battles when optimising. :)
	 * @param index
	 * @param data
	 */
	void setData(int index,void* data)
	{
/*		glBindBuffer(target,buffer);
        CHECK_OGL_ERRORS();
		glBufferSubData(target, index * elementSize,elementSize, data);
        CHECK_OGL_ERRORS();*/
		memcpy(((uint8_t*)bufferEmulation) + index * elementSize,data,elementSize);
	}

	/**
	 * Enables the buffer for use by the vertex shader.
	 * Should only be used with array buffers.
	 * @param streamIndex
	 * @param offset
	 */
	void Enable(int streamIndex,bool normalised,int offset)
	{
//		glBindBuffer(GL_ARRAY_BUFFER,buffer);
 //       CHECK_OGL_ERRORS();
		glVertexAttribPointer(streamIndex,elementCount,dataType,normalised,0, bufferEmulation);
        CHECK_OGL_ERRORS();
		glEnableVertexAttribArray(streamIndex);
        CHECK_OGL_ERRORS();
	}

	/**
	 * Only used with Element array types, IE vertex indices.
	 */
/*	void DrawTriList()
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,buffer);
		CHECK_OGL_ERRORS();
		glDrawElements(GL_TRIANGLES, count,dataType, 0);
		CHECK_OGL_ERRORS();
	}*/

private:
//	GLuint buffer;
	int count;
	int elementCount;
	int dataType;
	int target;
	int usage;
	int elementSize;
	int memSize;
	GLvoid *bufferEmulation;
};


/**
 * Buffer object for vertex position values.
 * @author richard
 *
 */
struct GLBufferXYZ : GLBuffer
{
	/**
	 * @param count Vertex count.
	 * @param staticDraw Set to true if the buffer is not going to be changed once filled with data. Improves performance.
	 */
	GLBufferXYZ(int count,bool staticDraw) : GLBuffer(count,3,GL_FLOAT,GL_ARRAY_BUFFER,staticDraw?GL_STATIC_DRAW:GL_DYNAMIC_DRAW)
	{
	}

	/**
	 * @param count Vertex count.
	 * @param staticDraw Set to true if the buffer is not going to be changed once filled with data. Improves performance.
	 */
	GLBufferXYZ(float* data,int length,bool staticDraw) : GLBuffer(length/3,3,GL_FLOAT,GL_ARRAY_BUFFER,staticDraw?GL_STATIC_DRAW:GL_DYNAMIC_DRAW)
	{
		setData(data);
	}

	/**
	 * Sets the x,y,z of an element at the index.
	 */
	void setElement(int index,Vector3& p)
	{
		setData(index,(void*)&p.x);
	}

	/**
	 * Sets the x,y,z of an element at the index.
	 */
	void setElement(int index,float x,float y,float z)
	{
		Vector3 p(x,y,z);
		setElement(index,p);
	}

};

/**
 * Buffer object for 2D vertex position values and 2D texture float coords.
 * @author richard
 *
 */
struct GLBufferXY : GLBuffer
{
	/**
	 * @param count Vertex count.
	 * @param staticDraw Set to true if the buffer is not going to be changed once filled with data. Improves performance.
	 */
	GLBufferXY(int count,bool staticDraw) : GLBuffer(count,2,GL_FLOAT,GL_ARRAY_BUFFER,staticDraw?GL_STATIC_DRAW:GL_DYNAMIC_DRAW)
	{
	}

	/**
	 * @param count Vertex count.
	 * @param staticDraw Set to true if the buffer is not going to be changed once filled with data. Improves performance.
	 */
	GLBufferXY(float* data,int length,bool staticDraw) : GLBuffer(length/2,2,GL_FLOAT,GL_ARRAY_BUFFER,staticDraw?GL_STATIC_DRAW:GL_DYNAMIC_DRAW)
	{
		setData(data);
	}

	/**
	 * Sets the x,y of an element at the index.
	 */
	void setElement(int index,float x,float y)
	{
		float d[] = {x,y};
		setData(index,d);
	}
};

/**
 * Buffer object for vertex colours.
 * @author richard
 *
 */
struct GLBufferColour : GLBuffer
{
	/**
	 * @param count Vertex count.
	 * @param staticDraw Set to true if the buffer is not going to be changed once filled with data. Improves performance.
	 */
	GLBufferColour(int count,bool staticDraw) : GLBuffer(count,4,GL_UNSIGNED_BYTE,GL_ARRAY_BUFFER,staticDraw?GL_STATIC_DRAW:GL_DYNAMIC_DRAW)
	{
	}

	/**
	 * @param count Vertex count.
	 * @param staticDraw Set to true if the buffer is not going to be changed once filled with data. Improves performance.
	 */
	GLBufferColour(int* data,int length,bool staticDraw):GLBuffer(length,4,GL_UNSIGNED_BYTE,GL_ARRAY_BUFFER,staticDraw?GL_STATIC_DRAW:GL_DYNAMIC_DRAW)
	{
		setData(data);
	}

	/**
	 * Sets the ARGB of an element at the index.
	 */
	void setElement(int index,int colour)
	{
		setData(index,&colour);
	}
};

/**
 * Buffer object for indices.
 * @author richard
 *
 */
struct GLBufferIndex : GLBuffer
{
	/**
	 * @param count Vertex count.
	 * @param staticDraw Set to true if the buffer is not going to be changed once filled with data. Improves performance.
	 */
	GLBufferIndex(int count,bool staticDraw) : GLBuffer(count,1,GL_UNSIGNED_SHORT,GL_ARRAY_BUFFER,staticDraw?GL_STATIC_DRAW:GL_DYNAMIC_DRAW)
	{
	}

	/**
	 * @param count Vertex count.
	 * @param staticDraw Set to true if the buffer is not going to be changed once filled with data. Improves performance.
	 */
	GLBufferIndex(short* data,int length,bool staticDraw):GLBuffer(length,1,GL_UNSIGNED_SHORT,GL_ARRAY_BUFFER,staticDraw?GL_STATIC_DRAW:GL_DYNAMIC_DRAW)
	{
		setData(data);
	}
};


} /* namespace BogDog */
#endif /* GLBUFFER_H_ */
