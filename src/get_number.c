#include "interprete_file.h"

uint8_t	get_number(t_in *in, uint32_t *n)
{
	switch (in->str[in->i])
	{
		case 'x':
		case 'h': return (hexa_ascii_to_uint(in, n));
		case 'o':
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
		case '$':
		case 'b': return (binary_ascii_to_uint(in, n));
		default:
			err_unexpected_char(in);
			skip_word(in);
		return (1);
	}
	return (0);
}
