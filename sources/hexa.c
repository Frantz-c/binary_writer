
#include "binw.h"

extern int		ishexa(int c, int maj)
{
	if (maj == 1)
		return ((c >= '0' && c <= '9') || (c >= 'A' && c <= 'F'));
	if (maj == 2)
		return ((c >= '0' && c <= '9')
			|| (c >= 'a' && c <= 'f') || (c >= 'A' && c <= 'F'));
	return ((c >= '0' && c <= '9') || (c >= 'a' && c <= 'f'));
}

static inline const char		*left_trim(const char *s)
{
	while (*s == ' ' || *s == '\t' || *s == '\n')
		s++;
	if (*s == '0')
	{
		while (*s == '0')
			s++;
		if (!ishexa(*s, HEXA_CHAR))
			s--;
		return (s);
	}
	if (!ishexa(*s, HEXA_CHAR))
		return (NULL);
	return (s);
}

static unsigned int				get_base_value(char c)
{
	if (c >= 'a' && c <= 'f')
		return (c - ('a' - 10));
	else if (c >= 'A' && c <= 'F')
		return (c - ('A' - 10));
	return (c - '0');
}

static inline unsigned int		strtoi(const char *s)
{
	unsigned int	n;

	n = get_base_value(*(s++));
	while (ishexa(*s, HEXA_CHAR))
	{
		n *= 16;
		n += get_base_value(*(s++));
	}
	return (n);
}

extern inline unsigned int		atoi_hexa(const char *s)
{
	if ((s = left_trim(s)) == NULL)
		return (0);
	return (strtoi(s));
}
