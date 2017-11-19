// ppm.h
// Riešenie IJC-DU1 príklad b) 26.3.2016
// Autor: Peter Grofčík FIT 1 BIA

#ifndef _PPM_
	#define _PPM_
	struct ppm { 
		unsigned xsize;
		unsigned ysize;
		char data[]; // RGB bajty, celkem 3*xsize*ysize
	};
	struct ppm * ppm_read(const char * filename);
	int ppm_write(struct ppm *p, const char * filename);
#endif
