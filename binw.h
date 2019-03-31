#ifndef BINW_H
# define BINW_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>

enum hexa_char
{
	HEXA_MIN, HEXA_MAJ, HEXA_BOTH
};

# define HEXA_CHAR	HEXA_BOTH


extern unsigned int		atoi_hexa(const char *s);
extern int				ishexa(int c, int maj);


extern int				isoctal(int c);
extern unsigned int		atoi_octal(const char *s);

extern int				isbin(int c);
extern unsigned int		atoi_binary(const char *s);

#endif
