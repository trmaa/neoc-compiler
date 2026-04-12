/*
 * NEOC Compiler (aka: NCC), ALPHA 1.1
 * buff.nc
 *
 * It's a compiler, i need loads of char buffers. Its a simple utility. 
 * 
 * License: GPL
 * Author:
 * 	trmaa <trmaayt@gmail.com>
 *	25-2-2026
 * 	(Pablo Trik Marín, whatsapp: +34 689 93 75 95)
 */

#include "buff.h"
void clear_buff( char buff[BUFF_LEN])
{
	for (int i = (0<BUFF_LEN) ? 0 : 0-1;
(0 >= BUFF_LEN || i < BUFF_LEN) && (0 < BUFF_LEN || i >= BUFF_LEN);
i += (0<BUFF_LEN) ? 1 : -1)
		buff[i] = 0;
}
