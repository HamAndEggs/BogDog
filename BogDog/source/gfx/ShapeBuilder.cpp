/*
 * ShapeBuilder.cpp
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

#include <stdio.h>
#include "gfx/ShapeBuilder.h"
#include "gfx/Mesh.h"

namespace BogDog
{

ShapeBuilder::ShapeBuilder()
{
}

ShapeBuilder::~ShapeBuilder()
{
}

/**
 *
 * @param v0
 * @param v1
 * @param v2
 * @param colour This is in ARGB format as per DX9, because that is what I like. GL is in ABGR. (grump but my code fixes that)
 * @return
 */
int ShapeBuilder::addFace(int v0,int v1,int v2,int colour)
{
	faces.PushBack()->Set(v0,v1,v2,colour,-1,-1,-1);
	return faces.GetSize()-1;
}

void ShapeBuilder::addQuad(int v0,int v1,int v2,int v3,int colour)
{
	faces.PushBack()->Set(v0,v1,v3,colour,-1,-1,-1);
	faces.PushBack()->Set(v1,v2,v3,colour,-1,-1,-1);
}

void ShapeBuilder::addQuad(int p0,int p1,int p2,int p3,int colour,int uv0,int uv1,int uv2,int uv3)
{
	faces.PushBack()->Set(p0,p1,p3,colour,uv0,uv1,uv3);
	faces.PushBack()->Set(p1,p2,p3,colour,uv1,uv2,uv3);
}

Mesh* ShapeBuilder::BuildMesh(bool wantColour,bool wantTex0)
{
	float* xyz = new float[faces.GetSize() * 3 * 3];
	int* colours = wantColour?new int[faces.GetSize() * 3]:NULL;
	float* uv0 = wantTex0?new float[faces.GetSize() * 3 * 2]:NULL;

	//Build vertex data.
	int n = 0;
	for(size_t fn = 0 ; fn < faces.GetSize() ; fn++ )
	{
		const Face& f = faces[fn];
		for( int i = 0 ; i < 3 ; i++ , n++ )
		{
			const Vector3& v = vertices[f.v[i]];
			xyz[(n*3) + 0] = v.x;
			xyz[(n*3) + 1] = v.y;
			xyz[(n*3) + 2] = v.z;

			if( colours != NULL )
			{
				colours[n] = f.colour;
			}

			if( uv0 != NULL )
			{
				if (f.uv0[i] > -1 )
				{
					int index = f.uv0[i];
					const Vector2& uv = this->uv0[index];
					index = (n*2);
					uv0[index + 0] = uv.x;
					uv0[index + 1] = uv.y;
				}
				else
				{
					uv0[(n*2) + 0] = v.x;
					uv0[(n*2) + 1] = v.y;
				}
			}
		}
	}

	Mesh* newMesh = new Mesh(xyz,uv0,colours,(int)faces.GetSize()*3);
	delete xyz;
	delete uv0;
	delete colours;

	return newMesh;
}

ShapeBuilder* ShapeBuilder::MakeBox(float x,float y,float z)
{
	x *= 0.5f;
	y *= 0.5f;
	z *= 0.5f;

	ShapeBuilder* builder = new ShapeBuilder();

	builder->addVertex(-x, y,-z);
	builder->addVertex( x, y,-z);
	builder->addVertex( x,-y,-z);
	builder->addVertex(-x,-y,-z);

	builder->addVertex(-x, y, z);
	builder->addVertex( x, y, z);
	builder->addVertex( x,-y, z);
	builder->addVertex(-x,-y, z);

	builder->addUV0(0, 0);//top left
	builder->addUV0(1, 0);//top right
	builder->addUV0(1, 1);//bottom right
	builder->addUV0(0, 1);//bottom left

	//Make faces.

	builder->addQuad(0,1,2,3,0xffff0000,0,1,2,3);//Front
	builder->addQuad(5,4,7,6,0xff00ff00,0,1,2,3);//Back
	builder->addQuad(1,5,6,2,0xff0000ff,0,1,2,3);//right
	builder->addQuad(4,0,3,7,0xffff00ff,0,1,2,3);//left
	builder->addQuad(0,4,5,1,0xffffffff,0,1,2,3);//top
	builder->addQuad(3,2,6,7,0xff00ffff,0,1,2,3);//bottom

	return builder;
}

} /* namespace BogDog */
