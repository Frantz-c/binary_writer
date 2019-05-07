
#include "binw.h"

void	realloc_buffer(char *bin[], int *binl)
{
	*binl += 128;
	if ((*bin = realloc(*bin, *binl)) == NULL)
	{
		fprintf(stderr, "memory allocation error\n");
		exit (1);
	}
}

void	add_byte_int(int byte, char *bin[], int *binl, int *cur)
{
	if (*cur == *binl - 1)
	{
		realloc_buffer(bin, binl);
	}
	(*bin)[(*cur)++] = byte;
}

char	*add_byte_octal(char buf[], char *bin[], int *binl, int *cur)
{
	if (*cur == *binl - 1)
	{
		realloc_buffer(bin, binl);
	}
	(*bin)[(*cur)++] = atoi_octal(buf);
	while (isoctal(*buf))
		buf++;
	return (buf);
}

char	*add_byte_decimal(char buf[], char *bin[], int *binl, int *cur)
{
	if (*cur == *binl - 1)
	{
		realloc_buffer(bin, binl);
	}
	(*bin)[(*cur)++] = atoi(buf);
	while (isdigit(*buf))
		buf++;
	return (buf);
}

char	*add_byte_binary(char buf[], char *bin[], int *binl, int *cur)
{
	if (*cur == *binl - 1)
	{
		realloc_buffer(bin, binl);
	}
	(*bin)[(*cur)++] = atoi_binary(buf);
	while (isbin(*buf))
		buf++;
	return (buf);
}

char	*add_byte_hexa(char buf[], char *bin[], int *binl, int *cur)
{
	if (*cur == *binl - 1)
	{
		realloc_buffer(bin, binl);
	}
	(*bin)[(*cur)++] = (char)atoi_hexa(buf);
	while (ishexa(*buf, HEXA_CHAR))
		buf++;
	return (buf);
}

void	add_character(char *buf[], char *bin[], int *binl, int *cur)
{
	char	*s = *buf;

	if (*s == '\\')
	{
		s++;
		if (*s == 'n')
		{
			s++;
			add_byte_int(012, bin, binl, cur);
		}
		else if (*s == 'r')
		{
			s++;
			add_byte_int(0xd, bin, binl, cur);
		}
		else if (*s == 't')
		{
			s++;
			add_byte_int(0x9, bin, binl, cur);
		}
		else if (*s == 'v')
		{
			s++;
			add_byte_int(0xb, bin, binl, cur);
		}
		else if (isoctal(*s))
		{
			s = add_byte_octal(s, bin, binl, cur);
		}
		else if (*s == 'x' && ishexa(s[1], HEXA_CHAR))
		{
			s++;
			s = add_byte_hexa(s, bin, binl, cur);
		}
		else
		{
			add_byte_int(*(s++), bin, binl, cur);
		}
	}
	else
	{
		add_byte_int(*(s++), bin, binl, cur);
	}
	*buf = s;
}

int		add_string(char *buf[], char *bin[], int *binl, int *cur)
{
	char	*s = *buf;

	while (*s && *s != '"')
	{
		add_character(&s, bin, binl, cur);
	}
	if (*s != '"')
		return (-1);
	*buf = s + 1;
	return (0);
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
				&& !ishexa(ptr[2], HEXA_CHAR) && ptr[2] != 'o')
		{
			ptr = add_byte_hexa(ptr, &loop, &loopl, &loopcur);
		}
		else if (isdigit(*ptr))
		{
			char *tmp = ptr;
			for (; isbin(*tmp); tmp++);
			if (*tmp == 'b')
				add_byte_binary(ptr, &loop, &loopl, &loopcur);
			else
			{
				for (; isoctal(*tmp); tmp++);
				if (*tmp == 'o')
					add_byte_octal(ptr, &loop, &loopl, &loopcur);
				else
				{
					for (; isdigit(*tmp); tmp++);
					if (*tmp != 'd')
						break ;
					add_byte_decimal(ptr, &loop, &loopl, &loopcur);
				}
			}
			ptr = tmp + 1;
		}
		else if (*ptr == '"')
		{
			ptr++;
			if (add_string(&ptr, &loop, &loopl, &loopcur) == -1)
				break ;
		}
		else if (*ptr == '\'')
		{
			ptr++;
			add_character(&ptr, &loop, &loopl, &loopcur);
			if (*ptr != '\'')
				break ;
			ptr++;
		}
		else if (*ptr == '(')
		{
			if (get_loops(&ptr, &loop, &loopl, &loopcur) == -1)
				goto ret_ff;
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

		if (*buf == '"')
		{
			buf++;
			if (add_string(&buf, &bin, &binl, &cur) == -1)
				break ;
		}
		else if (*buf == '\'')
		{
			buf++;
			add_character(&buf, &bin, &binl, &cur);
			if (*buf != '\'')
				break ;
			buf++;
		}
		else if (*buf == '(')
		{
			if (get_loops(&buf, &bin, &binl, &cur) == -1)
				break ;
		}
		else if (ishexa(buf[0], HEXA_CHAR) && ishexa(buf[1], HEXA_CHAR)
				&& !ishexa(buf[2], HEXA_CHAR) && buf[2] != 'o')
		{
			buf = add_byte_hexa(buf, &bin, &binl, &cur);
		}
		else if (isdigit(*buf))
		{
			char *tmp = buf;
			for (; isbin(*tmp); tmp++);
			if (*tmp == 'b')
				add_byte_binary(buf, &bin, &binl, &cur);
			else
			{
				for (; isoctal(*tmp); tmp++);
				if (*tmp == 'o')
					add_byte_octal(buf, &bin, &binl, &cur);
				else
				{
					for (; isdigit(*tmp); tmp++);
					if (*tmp != 'd')
						break ;
					add_byte_decimal(buf, &bin, &binl, &cur);
				}
			}
			buf = tmp + 1;
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
