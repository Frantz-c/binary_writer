
#include "binw.h"

extern int		isbin(int c)
{
	return (c == '1' || c == '0');
}

static inline const char		*left_trim(const char *s)
{
	while (*s == ' ' || *s == '\n' || *s == '\t')
		s++;
	if (*s == '0')
	{
		while (*s == '0')
			s++;
		if (!isbin(*s))
			s--;
		return (s);
	}
	if (!isbin(*s))
		return (NULL);
	return (s);
}

static inline unsigned int		strtoi(const char *s)
{
	unsigned int	n;

	n = *(s++) - '0';
	while (isbin(*s))
	{
		n *= 2;
		n += *(s++) - '0';
	}
	return (n);
}

extern inline unsigned int		atoi_binary(const char *s)
{
	if ((s = left_trim(s)) == NULL)
		return (0);
	return (strtoi(s));
}
