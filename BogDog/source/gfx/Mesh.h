/*
 * Mesh.h
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

#ifndef MESH_H_
#define MESH_H_

#include "GLHeaders.h"
#include "gl/GLBuffer.h"

namespace BogDog
{

struct Mesh
{

	Mesh(float* xyz,float* uv0,int* colours,int vertexCount);
	~Mesh();

	bool hasColours()
	{
		return mColours != NULL;
	}

	bool hasUV0()
	{
		return mUV0 != NULL;
	}

	/**
	 * Enables the vertex buffers and sets the streams.
	 */
	void Enable();

	/**
	 * Draws all the triangles.
	 */
	void Draw()
	{
		Draw(0,mXYZ->getCount());
	}

	/**
	 * Draws count triangles starting at first.
	 * @param first The triangle index to start drawing.
	 * @param count The number of triangles to draw.
	 */
	void Draw(int first,int count)
	{
		glDrawArrays(GL_TRIANGLES, first * 3,count);
	}

private:
	GLBufferXYZ* mXYZ;
	GLBufferColour* mColours;
	GLBufferXY* mUV0;
};

} /* namespace BogDog */
#endif /* MESH_H_ */
