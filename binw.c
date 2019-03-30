#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>

enum hexa_char
{
	HEXA_MIN, HEXA_MAJ, HEXA_BOTH
};

#define HEXA_CHAR	HEXA_BOTH

int		ishexa(int c, int maj)
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
	while (*s == ' ' || *s == '\n')
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

static inline unsigned int		ft_strtoi(const char *s)
{
	unsigned int	n;

	n = 0;
	while (1)
	{
		if (*s > 'f' || (*s > 'F' && *s < 'a')
				|| (*s > '9' && *s < 'A') || *s < '0')
			break ;
		n *= 16;
		n += get_base_value(*(s++));
	}
	return (n);
}

static inline unsigned int		atoi_hexa(const char *s)
{
	if ((s = left_trim(s)) == NULL)
		return (0);
	return (ft_strtoi(s));
}

char	*add_byte(char buf[], char *bin[], int *binl, int *cur)
{
	if (*cur == *binl - 1)
	{
		*binl += 128;
		if ((*bin = realloc(*bin, *binl)) == NULL)
		{
			fprintf(stderr, "memory allocation error\n");
			exit (1);
		}
	}
	(*bin)[(*cur)++] = (char)atoi_hexa(buf);
	return (buf + 2);
}

void	write_loop_content(char *bin[], int *binl, int *cur,
							char loop[], int loopcur, int n_times)
{
	if (*binl - *cur < loopcur * n_times)
	{
		*binl += (loopcur * n_times);
		*bin = realloc(*bin, *binl);
		if (*bin == NULL)
		{
			fprintf(stderr, "memory allocation error\n");
			exit(1);
		}
	}
	while (n_times)
	{
		memcpy(*bin + *cur, loop, loopcur);
		*cur += loopcur;
		n_times--;
	}
}

int		get_loops(char *buf[], char *bin[], int *binl, int *cur)
{
	char	*loop = malloc(128);
	char	*ptr = *buf + 1;
	int		loopl = 128;
	int		loopcur = 0;
	int		n_times;

	while (1)
	{
		while (*ptr && (*ptr == ' ' || *ptr == '\t' || *ptr == '\n'))
			ptr++;

		if (ishexa(ptr[0], HEXA_CHAR) && ishexa(ptr[1], HEXA_CHAR)
				&& !ishexa(ptr[2], HEXA_CHAR))
		{
			ptr = add_byte(ptr, &loop, &loopl, &loopcur);
		}
		else if (*ptr == '(')
		{
			if (get_loops(&ptr, &loop, &loopl, &loopcur) == -1)
				goto ret_ff;
			//ptr++;
		}
		else if (*ptr == ')' && ptr[1] == '.')
		{
			ptr += 2;
			if (!isdigit(*ptr))				goto ret_ff;
			if ((n_times = atoi(ptr)) == 0)	goto ret_00;
			while (isdigit(*(++ptr)));
			write_loop_content(bin, binl, cur, loop, loopcur, n_times);
			break ;
		}
		else goto ret_ff;
	}

	ret_00:
		free (loop);
		*buf = ptr;
		return (0);

	ret_ff:
		free (loop);
		return (-1);
}

int		print_binary_line(char *buf)
{
	static char				*bin = NULL;
	static int				binl;
	static int				line = 0;
	int						cur = 0;

	if (bin == NULL)
	{
		bin = malloc(128);
		binl = 128;
	}
	line++;

	while (1)
	{
		while (*buf && (*buf == ' ' || *buf == '\t' || *buf == '\n'))
			buf++;
		if (*buf == '#' || *buf == '\0') break ;

		if (*buf == '(')
		{
			if (get_loops(&buf, &bin, &binl, &cur) == -1)
				break ;
		}
		else if (ishexa(buf[0], HEXA_CHAR) && ishexa(buf[1], HEXA_CHAR)
				&& !ishexa(buf[2], HEXA_CHAR))
		{
			buf = add_byte(buf, &bin, &binl, &cur);
		}
		else
		{
			break ;
		}
	}
	if (*buf != '\0' && *buf != '#')
	{
		free(bin);
		char *lf = strchr(buf, '\n');
		if (lf)
			*lf = '\0';
		fprintf(stderr, "syntax error line %d\n-> \"%s\"\n", line, buf);
		return (-1);
	}
	write(1, bin, cur);
	return (0);
}

int		convert_file(FILE *file)
{
	char	*buf = NULL;
	int		bufl = 0;
	int		readl;

	if ((readl = getline(&buf, (size_t *)&bufl, file)) < 1)
	{
		fprintf(stderr, "empty file\n");
		fclose(file);
		return (1);
	}

	do
	{
		if (print_binary_line((char *)buf) == -1)
			break ;
	}
	while ((readl = getline(&buf, (size_t *)&bufl, file)) > 0);
	
	fclose(file);
	free(buf);
	return ((readl > 0) ? 1 : 0);
}

int		main(int argc, char *argv[])
{
	FILE	*file;

	if (argc < 2)
	{
		file = stdin;
	}
	else if ((file = fopen(argv[1], "r")) == NULL)
	{
		fprintf(stderr, "can't open %s\n", argv[1]);
		return (1);
	}

	return (convert_file(file));
}
