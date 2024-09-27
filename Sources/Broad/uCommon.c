/*
 * uCommon.c
 *
 *  Created on: 2024Äê5ÔÂ7ÈÕ
 *      Author: zhonggaowei
 */

#include "uCommon.h"
#include "string.h"
#include <stdarg.h>
#include "stdlib.h"
#include "stdio.h"

void uCanPrintf(const char *format, ...){
	char buffer[200] = {0};
	int count;
	va_list args;
	va_start(args, format);
	count = vsnprintf(buffer + 1, sizeof(buffer) - 2, format, args);
	va_end(args);
	buffer[0] = 0x02;
	buffer[count + 1] = 0x03;

	uCanDebug(buffer);
}

unsigned int fourCharToInt(unsigned char a, unsigned char b, unsigned char c, unsigned char d)
{
	unsigned int ret_val = 0;
	ret_val  =  a;
	ret_val <<= 8;
	ret_val |=  b;
	ret_val <<= 8;
	ret_val |=  c;
	ret_val <<= 8;
	ret_val |=  d;

	return ret_val;
}
