// error.c
// Riešenie IJC-DU1 príklad b) 26.3.2016
// Autor: Peter Grofčík FIT 1 BIA

#include "error.h"

#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>

void warning_msg(const char *fmt, ...)
{
	va_list massage;vfprintf(stderr,"CHYBA: ",massage);

	va_start(massage, fmt);vfprintf(stderr,fmt,massage);

	va_end(massage);
}

void fatal_error(const char *fmt, ...)
{
	va_list massage;vfprintf(stderr,"CHYBA: ",massage);

        va_start(massage, fmt);vfprintf(stderr,fmt,massage);

        va_end(massage);
	exit(1);

}
