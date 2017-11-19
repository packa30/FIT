// bit_array.h
// Riešenie IJC-DU1 príklad a) 26.3.2016
// Autor: Peter Grofčík FIT 1 BIA


#ifndef _BIT_ARRAY_T
        #define _BIT_ARRAY_T

#include "error.h"
#include <stdio.h>
#include <limits.h>

typedef unsigned long unsigned_bit;
typedef unsigned_bit bit_array_t[];


#define ba_create(meno_pola,value) unsigned_bit meno_pola [1+ ((value)/(CHAR_BIT*sizeof(unsigned_bit)-1)) + (((value)%(CHAR_BIT*sizeof(unsigned_bit)-1)) ? 1 :0)]={value}

#ifndef USE_INLINE
	#define ba_size(meno_pola) meno_pola[0]
#else
	static inline unsigned_bit ba_size(bit_array_t meno_pola)
		{
			return meno_pola[0];
		}
#endif

#ifndef USE_INLINE
	#define ba_set_bit(meno_pola,index,vyraz) \
		if(meno_pola[0]<=(unsigned_bit)(index)) \
		{ \
			fatal_error("Nepovoleny rozsah 0-%ld",(long)meno_pola[0]); \
		} \
		if (vyraz) \
		{ \
			(meno_pola[1+(index)/(CHAR_BIT*sizeof(unsigned_bit)-1)] |= ((unsigned_bit)1 << (index)%(CHAR_BIT*sizeof(unsigned_bit)-1))); \
		} \
		else \
		{ \
			(meno_pola[1+(index)/(CHAR_BIT*sizeof(unsigned_bit)-1)] &= ~((unsigned_bit)1 << (index)%(CHAR_BIT*sizeof(unsigned_bit)-1))); \
		}
#else
	static inline void ba_set_bit(bit_array_t meno_pola, unsigned_bit index, unsigned_bit vyraz)
	{
		if(meno_pola[0]<=(unsigned_bit)(index))
		{
			fatal_error("Nepovoleny rozsah 0-%ld",(long)meno_pola[0]);
		}
			else
			{
				if(vyraz)
					meno_pola[1+(index)/(CHAR_BIT*sizeof(unsigned_bit)-1)] |= ((unsigned_bit)1 << (index)%(CHAR_BIT*sizeof(unsigned_bit)-1));
				else
					meno_pola[1+(index)/(CHAR_BIT*sizeof(unsigned_bit)-1)] &= ~((unsigned_bit)1 << (index)%(CHAR_BIT*sizeof(unsigned_bit)-1));

			}
			return;
	}
#endif

#ifndef USE_INLINE
	#define ba_get_bit(meno_pola,index) \
		((meno_pola[0]<=(unsigned_bit)(index)) \
		?(fatal_error("Nepovoleny rozsah 0-%ld",(long)meno_pola[0]),0)\
		:((meno_pola[1+(index)/(CHAR_BIT*sizeof(unsigned_bit)-1)] & ((unsigned_bit)1 << (index)%(CHAR_BIT*sizeof(unsigned_bit)-1)))>0))
#else
	static inline unsigned_bit ba_get_bit(bit_array_t meno_pola, unsigned_bit index)
	{
		if(meno_pola[0]<=(unsigned_bit)(index))
		{
			fatal_error("Nepovoleny rozsah 0-%ld",(long)meno_pola[0]);
			return 0;
		}
			else
			{
				return ((meno_pola[1+(index)/(CHAR_BIT*sizeof(unsigned_bit)-1)] & ((unsigned_bit)1 << (index)%(CHAR_BIT*sizeof(unsigned_bit)-1)))>0);
			}
	}
#endif


#define DU1_GET_BIT(p,i) (((p)[(i)/(CHAR_BIT*sizeof(*p)-1)] & (1 << (i)%(CHAR_BIT*sizeof(*p)-1)))>0)
#define DU1_SET_BIT(p,i,b) \
	if(b) \
	p[(i)/(CHAR_BIT*sizeof(*p)-1)] |= (1 << (i)%(CHAR_BIT*sizeof(*p)-1)); \
	else \
	p[(i)/(CHAR_BIT*sizeof(*p)-1)] &= ~(1 << (i)%(CHAR_BIT*sizeof(*p)-1));
#endif
