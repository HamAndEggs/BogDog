/*
 * SinCos.h
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

#ifndef SINCOS_H_
#define SINCOS_H_

#include <stdint.h>
#include "../Common.h"

namespace BogDog
{

// ---------------------------------------------------------------------------

enum{DEG_0		= 0};
enum{DEG_1		= 182};
enum{DEG_45		= 0x2000};
enum{DEG_90		= 0x4000};
enum{DEG_135	= 0x6000};
enum{DEG_180	= 0x8000};
enum{DEG_225	= 0xA000};
enum{DEG_270	= 0xC000};
enum{DEG_315	= 0xE000};
enum{DEG_360	= 0xffff};

/*!
	Return the cosine of the specified angle.
	@param	pA	Angle range 0 to 360 degrees.
	@return		Cosine of angle
*/
float Cos(Angle pA);

/*!
	Return the sine of the specified angle.
	@param	pA	Angle range 0 to 360 degrees.
	@return		Sine of angle.
*/
float Sin(Angle pA);

/*!
	Calculate an angle which is a specified number of steps between two other
	angles, using the shortest step distance.  For instance, 100 to 110 would
	rotate clockwise, but 110 to 100 would rotate anticlockwise.
	@param[in,out]	pAngle	Start angle.  Note that the result is also returned in this variable.
	@param			pTarget	Target angle in AGS format.
	@param			pStep	Number of degrees to step, in AGS format.
	@return					True if calculated pAngle is equal to pTarget, else false.
*/
bool RotateShortestDistance(Angle& pAngle,Angle pTarget,Angle pStep);

/*!
 * Util function.
 */
void MakeCosSinTable();


} /* namespace BogDog */
#endif /* SINCOS_H_ */
