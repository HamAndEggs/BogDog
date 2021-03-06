/*
 * GLShaderColour.cpp
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

#include "gl/GLShaderColour.h"

namespace BogDog
{
static const char* vertexShader = ""	\
"uniform mat4 u_proj_cam;\n"	\
"uniform mat4 u_trans;\n"	\
"uniform vec4 u_global_colour;\n" \
"attribute vec4 a_xyz;\n"	\
"attribute vec4 a_col;\n"	\
"varying vec4 v_col;\n"	\
"void main(void)\n"	\
"{\n"	\
"	v_col = u_global_colour * a_col;\n"	\
"	gl_Position = u_proj_cam * (u_trans * a_xyz);\n"	\
"}\n";

static const char *pixelShader = ""	\
"varying vec4 v_col;\n"	\
"void main(void)\n"	\
"{\n"	\
"	gl_FragColor = v_col;\n"	\
"}\n";

GLShaderColour* GLShaderColour::Allocate()
{
	return new GLShaderColour();
}


GLShaderColour::GLShaderColour()
{
	Create(vertexShader,pixelShader);
}

GLShaderColour::~GLShaderColour()
{
}

} /* namespace BogDog */
