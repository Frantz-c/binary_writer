
#include "binw.h"

extern int		isoctal(int c)
{
	return ((c >= '0' && c <= '7'));
}

static inline const char	*left_trim(const char *s)
{
	while (*s == ' ' || *s == '\t' || *s == '\n')
		s++;
	if (*s == '0')
	{
		s++;
		while (*s == '0')
			s++;
		return (s);
	}
	else if (isoctal(*s))
		return (s);
	return (NULL);
}

static inline unsigned int		strtoi(const char *s)
{
	unsigned int	n;

	n = 0;
	while (1)
	{
		if (*s > '7' || *s < '0')
			break ;
		n *= 8;
		n += *(s++) - '0';
	}
	return (n);
}

extern inline unsigned int		atoi_octal(const char *s)
{
	if ((s = left_trim(s)) == NULL)
		return (0);
	return (strtoi(s));
}
