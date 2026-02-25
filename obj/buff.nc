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

use "buff.nh"


fn clear_buff(buff[BUFF_LEN]: char) ~void  {

	for let i in 0..BUFF_LEN
		buff[i] = 0;
}
