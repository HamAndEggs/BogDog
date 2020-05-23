/*
 *
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


#ifndef __BOX_H__
#define __BOX_H__

#include "Vector3.h"

namespace BogDog
{
// ---------------------------------------------------------------------------
#define BOX_CORNER_MAX_Z	(1<<0)
#define BOX_CORNER_MAX_Y	(1<<1)
#define BOX_CORNER_MAX_X	(1<<2)

enum
{	// The bits define which axis is positive, that is comes from the max vector and not the min vector.
	//		   bit 2 bit 1 bit 0
	BOX_CORNER_MIN_X_MIN_Y_MIN_Z = 0,
	BOX_CORNER_MIN_X_MIN_Y_MAX_Z = 1,
	BOX_CORNER_MIN_X_MAX_Y_MIN_Z = 2,
	BOX_CORNER_MIN_X_MAX_Y_MAX_Z = 3,
	BOX_CORNER_MAX_X_MIN_Y_MIN_Z = 4,
	BOX_CORNER_MAX_X_MIN_Y_MAX_Z = 5,
	BOX_CORNER_MAX_X_MAX_Y_MIN_Z = 6,
	BOX_CORNER_MAX_X_MAX_Y_MAX_Z = 7,
};

struct Bounds
{
	Vector3 min,max;

	Bounds(){};
	Bounds(const Vector3 &pMin,const Vector3 &pMax);
	Bounds(const Vector3 &pCenter,float pSize);
	Bounds(const Vector3 &pSize);
	Bounds(float pSize);

	bool IsNotNULL()const;
	bool IsValid()const;

	//Copies.
	Bounds& operator = ( const Bounds &pBounds ){min = pBounds.min;max = pBounds.max;return *this;}

	Bounds& operator += ( const Vector3 &v ){min += v;max += v;return *this;}
    Bounds& operator -= ( const Vector3 &v ){min -= v;max -= v;return *this;}
    Bounds& operator *= ( const Vector3 &v ){min *= v;max *= v;return *this;}
    Bounds& operator /= ( const Vector3 &v ){min /= v;max /= v;return *this;}
    Bounds& operator *= ( float f){min *= f;max *= f;return *this;};
    Bounds& operator /= ( float f){f = 1.0f/f;min *= f;max *= f;return *this;};

	//Makes the box big enough to encompuss the point.
	void Grow(const Vector3 *pPoint);
	void Grow(float pX,float pY,float pZ);
	void Grow(const Bounds *pBounds);

	void Scale(float pScale);//A value of one willl not change it, scales around the center point.
	void Scale(float pX_scale,float pY_scale,float pZ_scale);//A value of one willl not change it, scales around the center point.

	//Sets min max to the first point and calls Grow on the rest.
	void Make(const Vector3 *pPoints,int pNum_points);
	void Make(const Vector3 &pCenter,const Vector3 &pSize);
	void Make(const Vector3 &pCenter,float pSize);
	void Make(const Vector3 &pSize);
	void Make(float pCenter_x,float pCenter_y,float pCenter_z,float pSize_x,float pSize_y,float pSize_z);
	void Make(float pSize_x,float pSize_y,float pSize_z);
	void Make(float pSize);

	//Get functions
	void GetFacePosX(Vector3 *rTop_left,Vector3 *rTop_right,Vector3 *rBottom_right,Vector3 *rBottom_left)const;
	void GetFaceNegX(Vector3 *rTop_left,Vector3 *rTop_right,Vector3 *rBottom_right,Vector3 *rBottom_left)const;
	void GetFacePosY(Vector3 *rTop_left,Vector3 *rTop_right,Vector3 *rBottom_right,Vector3 *rBottom_left)const;
	void GetFaceNegY(Vector3 *rTop_left,Vector3 *rTop_right,Vector3 *rBottom_right,Vector3 *rBottom_left)const;
	void GetFacePosZ(Vector3 *rTop_left,Vector3 *rTop_right,Vector3 *rBottom_right,Vector3 *rBottom_left)const;
	void GetFaceNegZ(Vector3 *rTop_left,Vector3 *rTop_right,Vector3 *rBottom_right,Vector3 *rBottom_left)const;
	void GetSize(Vector3 *rSize)const;
	void GetCenter(Vector3 *rCenter)const;
	void GetCorner(int pCorner,Vector3 *rCorner)const;
	void GetCorner(int pCorner,Vector3 *rCorner,Vector3 *rX_dir,Vector3 *rY_dir,Vector3 *rZ_dir)const;

	//Matrix mul stuff, used to make axis aligned boxes.
	Bounds *MatrixMul(const Matrix *pMatrix);//Returns the 'this' pointer.
	Bounds *MatrixMul(const Matrix *pMatrix,const Bounds *pBounds);//Returns the 'this' pointer.

	//Some intersection functions.
	int IntersectsAA(const Bounds &pBounds)const;//Returns true if pBounds intersects 'this'.
	//Returns true if pBounds intersects 'this', fills pSeperating_axis with the normalised seperating axis.( not propper norm, just each axis normed )
	int IntersectsAA(const Bounds &pBounds,Vector3 &rSeperating_axis)const;

};
// ---------------------------------------------------------------------------
};//namespace BogDog{

#endif//#ifndef __BOX_H__
