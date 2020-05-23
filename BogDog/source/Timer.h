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

#ifndef __TIMER_H__
#define __TIMER_H__

#include <sys/time.h>

namespace BogDog
{

struct Timer
{
	void Start()
	{
		gettimeofday(&start,NULL);
	}

	void Stop()
	{
		gettimeofday(&stop,NULL);
		microseconds = (stop.tv_sec - start.tv_sec) * (uint64_t)1000000;
		microseconds += (stop.tv_usec - start.tv_usec);
	}

	uint64_t GetMicroseconds()
	{
		return microseconds;
	}

	float GetSeconds()
	{
		double t = ((double)microseconds) / 1000000.0;
		return (float)t;
	}

private:
	struct timeval start,stop;
	uint64_t microseconds;

};

};

#endif //ifndef __TIMER_H__
