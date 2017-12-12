/*
 * Ex1.c
 *
 *  Created on: Nov 14, 2017
 *      Author: hanani corney
 *      id: 203990924
 */

#include <stdio.h>

/*
 * This function checks which system compiled the code,
 * little endian or big endian.
 */
int is_little_endian()
{
	   unsigned int i = 0xAB56;
	   char *c = (char*)&i;
	   //little endian
	   if (*c)
	   {
	       return 1;
	   }
	   //big endian
	   else
	   {
	       return 0;
	   }
}

/*
 * This function receives two numbers x and y,
 * and inserts y's lsb to be x's lsb.
 */
unsigned long merge_bytes(unsigned long x, unsigned long int y)
{
	unsigned char lsbY;
	unsigned long mask = 0xffffffffffffff00;
	//extracting y's lsb
	lsbY = y & 0xff;
	//putting zeros at x's lsb
	x &= mask;
	//putting the lsb of y in the lsb of x
	x |= lsbY;
	return x;
}

/*
 * This function receives x, b, i.
 * It inserts b, in x at the i'th place, where 0 is the lsb
 */
unsigned long put_byte(unsigned long x, unsigned char b, int i)
{
	int j = 0;
	unsigned long mask2 = 0xffffffffffffff00, mask1 = 0;
	mask1 += b;
	for(j = 1; j <= i; j++)
	{
		//moving b along mask 1
		mask1 <<= 8;
		//moving the zeros along mask 2
		mask2 <<= 8;
		//make sure the lsbs are 1
		mask2 += 0xff;
	}
	x &= mask2;
	return x + mask1;
}
