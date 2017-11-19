// primes.c
// Riešenie IJC-DU1 príklad a) 26.3.2016
// Autor: Peter Grofčík FIT 1 BIA


#include <stdio.h>
#include "error.h"
#include "bit_array.h"
#include "eratosthenes.h"

int main()
{
	ba_create(p,202000001);
	Eratosthenes(p);
	int last=10;
	unsigned_bit prime[last];

	unsigned_bit k;
	k=(ba_size(p)-1);
	while(k>1 && last>0)
	{
		if(ba_get_bit(p,k)==0)
		{
			prime[--last]=k;
		}
		k--;
	}
	int l=0;
	while(l<10)
	{
		printf("%lu\n",prime[l]);
		l++;
	}
	return 0;
}
