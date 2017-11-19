/*
*       Subor : hash_function.c
*       Riešenie IJC-DU2, priklad 2
*       Autor: Peter Grofčík, FIT 1BIT
*/


unsigned int hash_function(const char *str, unsigned htab_size)
{
	unsigned int h=0;
	const unsigned char *p;
	for(p=(const unsigned char*)str; *p!='\0'; p++)
		h = 65599*h + *p;

	return h % htab_size;
}
