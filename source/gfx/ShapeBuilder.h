/*
 * ShapeBuilder.h
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

#ifndef SHAPEBUILDER_H_
#define SHAPEBUILDER_H_

#include <vector>
#include "maths/Vector2.h"
#include "maths/Vector3.h"
#include "Mesh.h"
#include "DynamicBuffer.h"

namespace BogDog
{

struct ShapeBuilder
{
	struct Face
	{
		int v[3];
		int colour;//As per GLES ABGR
		int uv0[3];

		void Set(int v0,int v1,int v2,int pColour,int uv00,int uv01,int uv02)
		{
			v[0] = v0;
			v[1] = v1;
			v[2] = v2;
			colour = pColour;
			uv0[0] = uv00;
			uv0[1] = uv01;
			uv0[2] = uv02;
		}
	};

	DynamicBuffer<Face> faces;
	DynamicBuffer<Vector3> vertices;
	DynamicBuffer<Vector2> uv0;

	ShapeBuilder();
	virtual ~ShapeBuilder();

	int addVertex(float x,float y,float z)
	{
		vertices.PushBack()->Set(x,y,z);
		return vertices.GetSize()-1;
	}

	int addUV0(float x,float y)
	{
		uv0.PushBack()->Set(x,y);
		return uv0.GetSize()-1;
	}

	/**
	 *
	 * @param v0
	 * @param v1
	 * @param v2
	 * @param colour This is in GL format ABGR. (grump I prefer DX ARGB)
	 * @return
	 */
	int addFace(int v0,int v1,int v2,int colour);

	void addQuad(int v0,int v1,int v2,int v3,int colour);

	void addQuad(int p0,int p1,int p2,int p3,int colour,int uv0,int uv1,int uv2,int uv3);

	Mesh* BuildMesh(bool wantColour,bool wantTex0);

	static ShapeBuilder* MakeBox(float x,float y,float z);

};

} /* namespace BogDog */
#endif /* SHAPEBUILDER_H_ */
