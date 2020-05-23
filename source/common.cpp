/*
 * common.cpp
 *
 *  Created on: 20 Jun 2012
 *      Author: richard
 */

#include <string.h>
#include "Common.h"

bool striend(const char* str,const char* suffix)
{
	if( str == NULL || suffix == NULL )
	{
		return 0;
	}

	size_t str_len = strlen(str);
	size_t suffix_len = strlen(suffix);

	if(suffix_len > str_len)
	{
		return 0;
	}

	return 0 == strncasecmp( str + str_len - suffix_len, suffix, suffix_len );
}
