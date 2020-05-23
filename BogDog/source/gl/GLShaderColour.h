/*
 * GLShaderColour.h
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

#ifndef __GLSHADERCOLOUR_H__
#define __GLSHADERCOLOUR_H__

#include "GLShader.h"

namespace BogDog
{

struct GLShaderColour : GLShader
{
	static GLShaderColour* Allocate();
	virtual ~GLShaderColour();

private:
	/*!
	 * Done to force the use of the new else you'll get shaders being made before GL is started.
	 */
	GLShaderColour();

};

} /* namespace BogDog */
#endif /* __GLSHADERCOLOUR_H__ */
