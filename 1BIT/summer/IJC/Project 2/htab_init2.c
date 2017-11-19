#include "htable.h"

  htab_t *htab_init2( unsigned int size, unsigned int ( *hashfunction)( const char *str, unsigned htab_size ) )
	{
		htab_t *new_tb = malloc( size * sizeof(struct htab_listitem * ) + sizeof( htab_t ) );
		if (new_tb == NULL)
			return NULL;
	
		for (unsigned int k = 0; k < size; k++)
			{
				new_tb->ptr[k] = NULL;
			}

		new_tb->htab_size = size;
		new_tb->n = 0;
		new_tb->hash_fun_ptr = hashfunction;
	
		return new_tb;
	}
