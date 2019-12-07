#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdint.h>
#include <ctype.h>
#include <fcntl.h>

#define length_ok(file,n)		((file->len - file->i) >= n)
#define length_not_ok(file,n)	((file->len - file->i) < n)
#define len_is_0(file)			(file->i >= file->len)
#define len_is_not_0(file)		(file->i < file->len)

#define ALL_BASES	0x0u
#define HEXA_BASE	0x1u
#define OCTAL_BASE	0x2u
#define BINARY_BASE	0x3u

#define LOOP		0x1u
#define DEFAULT		0x0u

typedef struct	s_fin
{
	uint8_t		*str;
	uint32_t	len;
	uint32_t	i;
	uint32_t	line;
	uint32_t	base;
	const char	*name;
}
t_fin;

typedef struct	s_fout
{
	uint8_t		*str;
	uint32_t	len;
	uint32_t	i;
	const char	*name;
	uint8_t		*buf;
	uint32_t	buflen;
	uint32_t	j;
	uint8_t		status;	// LOOP or DEFAULT
	uint8_t		parent_lvl;
}
t_fout;

typedef struct	s_file
{
	uint8_t		*str;
	uint32_t	len;
	uint32_t	i;
	uint32_t	line;
	uint32_t	base;
	const char	*name;
	uint8_t		*buf;
	uint32_t	buflen;
	uint32_t	j;
	uint8_t		status;	// LOOP or DEFAULT
	uint8_t		parent_lvl;
}
t_file;

uint8_t	(*router[0x100])(t_file*,t_file*) = {
	NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
	NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
	NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
	NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
	NULL, NULL, &get_string, &skip_comment,					// ' ' '!' '"' '#'
	NULL, &get_directive, NULL, &get_character,				// '$' '%' '&' '\''
	&get_loop, NULL, NULL, NULL,							// '(' ')' '*' '+'
	NULL, NULL, NULL, NULL,									// ',' '-' '.' '/'
	&get_octal, &get_decimal, &get_decimal, &get_decimal,	// '0' '1' '2' '3'
	&get_decimal, &get_decimal, &get_decimal, &get_decimal, // '4' '5' '6' '7'
	&get_decimal, &get_decimal, NULL, &skip_comment,		// '8' '9' ':' ';'
	&set_base, NULL, NULL, NULL,							// '<' '=' '>' '?'
	NULL, NULL, NULL, NULL,									// '@' 'A' 'B' 'C'
	NULL, NULL, NULL, NULL,									// 'D' 'E' 'F' 'G'
	NULL, NULL, NULL, NULL,									// 'H' 'I' 'J' 'K'
	NULL, NULL, NULL, NULL,									// 'L' 'M' 'N' 'O'
	NULL, NULL, NULL, NULL,									// 'P' 'Q' 'R' 'S'
	NULL, NULL, NULL, NULL,									// 'T' 'U' 'V' 'W'
	NULL, NULL, NULL, &get_auto_padding_block,				// 'X' 'Y' 'Z' '['
	NULL, NULL, NULL, NULL,									// '\\' ']' '^' '_'
	NULL, NULL, NULL, NULL,									// '`' 'a' 'b' 'c'
	NULL, NULL, NULL, NULL,									// 'd' 'e' 'f' 'g'
	&get_hexadecimal, NULL, NULL, NULL,						// 'h' 'i' 'j' 'k'
	NULL, NULL, NULL, NULL,									// 'l' 'm' 'n' 'o'
	NULL, NULL, NULL, NULL,									// 'p' 'q' 'r' 's'
	NULL, NULL, NULL, NULL,									// 't' 'u' 'v' 'w'
	&get_hexadecimal, NULL, NULL, NULL,						// 'x' 'y' 'z' '{'
	NULL, NULL, NULL, NULL									// '|' '}' '~' DEL
};

// set_base modify router

int		get_file_content(t_file *in, const char *filename)
{
	int	fd;

	if ((fd = open(filename, O_RDONLY)) == -1)
		goto __open_error;

	if ((in->len = (uint32_t)lseek(fd, 0, SEEK_END)) == (off_t)-1)
		goto __lseek_error;
	if (in->len == 0)
		goto __empty_file;
	lseek(fd, 0, SEEK_SET);


	if ((in->str = malloc(in->len)) == NULL)
		goto __no_memory;


	if (read(fd, in->str, in->len) != in->len)
		goto __read_error;

	close(fd);
	return (0);

// errors
	__no_memory:
	dprintf(STDERR_FILENO, "no more memory enable\n");
	goto __ret_error;
	__empty_file:
	dprintf(STDERR_FILENO, "file \"%s\" is empty\n", filename);
	goto __ret_error;
	__read_error:
	dprintf(STDERR_FILENO, "can't read the entire file \"%s\"\n",
			filename);
	__ret_error:
	close(fd);
	return (1);

	__open_error:
	dprintf(STDERR_FILENO, "can't open file \"%s\"\n", filename);
	return (1);
}

int		interprete_file(const char *filename)
{
	t_file		in = {NULL,0,0,1,ALL_BASES,filename,NULL,0,0,0,0};
	t_file		out = {NULL,0,0,0,0,NULL,NULL,0,0,0,0};
	uint32_t	error = 0;

	if (get_file_content(&in, filename) != 0)
		return (1);

	while (in.i < in.len)
	{
		// skip spaces
		while (1)
		{
			if (in.str[in.i] == '\n')
				in.line++;
			else if (in.str[in.i] != ' ' && in.str[in.i] != '\t')
				break;
			in.i++;
			if (in.i == i.len)
				goto __end_loops;
		}

		if (router[in.str[in.i]] == NULL)
			error += err_unexpected_char(in);
		else
			error += router[in.str[in.i]](&in, &out);
	}

	__end_loops:
	if (out.str == NULL)
	{
		dprintf(STDERR_FILENO, "empty file or not a valid bwl file\n");
		free(in.str);
		return (1);
	}
	write(STDOUT_FILENO, out.str, out.len);
	free(out.str);
	free(in.str);

	return (0);
}
