// erastothenes.c
// Riešenie IJC-DU1 príklad a) 26.3.2016
// Autor: Peter Grofčík FIT 1 BIA


#include <stdio.h>
#include <math.h>
#include "eratosthenes.h"

void Eratosthenes(bit_array_t nazov_pola)
{
	nazov_pola[1]=1;
	unsigned_bit a=2;
	while(a<=sqrt(nazov_pola[0]))
	{
		if (ba_get_bit(nazov_pola,a)==0)
		{
			unsigned_bit b=2;
			while(a*b<nazov_pola[0])
			{
				ba_set_bit(nazov_pola,a*b,1);
				b++;
			}
		}
		a++;
	}
}


