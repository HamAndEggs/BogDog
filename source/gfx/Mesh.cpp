/*
 * Mesh.cpp
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

#include "gfx/Mesh.h"
#include "gl/GLShader.h"

namespace BogDog
{

Mesh::Mesh(float* xyz,float* uv0,int* colours,int vertexCount)
{
	mXYZ = new GLBufferXYZ(xyz,vertexCount*3,true);
	mColours = colours == NULL?NULL:new GLBufferColour(colours,vertexCount,true);
	mUV0 = uv0 == NULL?NULL:new GLBufferXY(uv0,vertexCount*2,true);
}

void Mesh::Enable()
{
	mXYZ->Enable(ATTRIB_POS,false, 0);
	if( mColours != NULL )
	{
		mColours->Enable(ATTRIB_COLOUR,true, 0);
	}
	if( mUV0 != NULL )
	{
		mUV0->Enable(ATTRIB_UV0,true, 0);
	}
}

Mesh::~Mesh()
{

}

} /* namespace BogDog */
