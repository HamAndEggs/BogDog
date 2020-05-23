
/*
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
 *
 *	Stuff that gets user a lot, like some math defines and base types.
 */

#ifndef __COMMON_H__
#define __COMMON_H__

#include <inttypes.h>
#include <stddef.h>
#include <sys/time.h>

typedef uint16_t Angle;

////////////////////////////////////////////////////////////////////////////////
//	Math constants
////////////////////////////////////////////////////////////////////////////////
#define BD_DEG2RAD						0.01745329251994329547f				//!< Multiplier to convert degrees to radians
#define BD_PI							3.1415926535897932384626433832795f	//!< PI as a floating point value
#define BD_PHI							1.6180339887498948482045868343656f	//!< Golden ratio as a floating point value

////////////////////////////////////////////////////////////////////////////////
//	Handy string functions
////////////////////////////////////////////////////////////////////////////////

/*
 * Checks if str ends in suffix, ignores case.
 * Returns true if it does.
 */
bool striend(const char* str,const char* suffix);

#endif //#ifndef __COMMON_H__
