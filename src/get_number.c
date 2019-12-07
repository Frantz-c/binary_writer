#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdint.h>
#include <ctype.h>
#include <fcntl.h>

int		get_number(t_file *in, uint32_t *n)
{
	switch (in->str[in->i])
	{
		case 'x':
		case 'h': return (hexa_ascii_to_uint(in, n));
		case '0': return (octal_ascii_to_uint(in, n));
		case '1':
		case '2':
		case '3':
		case '4':
		case '5':
		case '6':
		case '7':
		case '8':
		case '9': return (decimal_ascii_to_uint(in, n));
		case 'b': return (binary_ascii_to_uint(in, n));
		default:
			dprintf
			(
				STDERR_FILENO,
				"file %s:l%u:\tunexpected char '%c'\n",
				in->name, in->line, in->str[in->i]
			);
			skip_word(in);
		return (-1);
	}
	return (0);
}
