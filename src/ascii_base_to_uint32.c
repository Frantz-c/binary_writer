#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdint.h>
#include <ctype.h>
#include <fcntl.h>

// coder skip_word()

#define is_binary_char(c)\
	(c == '1' || c == '0')

#define not_binary_char(c)\
	(c != '1' && c != '0')

#define is_octal_char(c)\
	(c >= '0' && c <= '7')

#define not_octal_char(c)\
	(c < '0' || c > '7')

#define is_decimal_char(c)\
	(c >= '0' && c <= '9')

#define not_decimal_char(c)\
	(c < '0' || c > '9')

#define is_hex_char(c)\
	((c >= 'a' && c <= 'f') || (c >= 'A' && c <= 'F')\
	|| (c >= '0' && c <= '9'))

#define	not_hex_char(c)\
	(c > 'f' || (c < 'a' && c > 'F')\
	|| (c < 'A' && c > '9') || c < '0')

static uint8_t	get_hex_digit(uint8_t c)
{
	if (c >= 'a')
		return (c - 'a');
	if (c >= 'A')
		return (c - 'A');
	return (c - '0');
}

int				hexa_ascii_to_uint(t_file *in, uint32_t *n)
{
	in->i++;
	if (length_is_0(in))
	{
		in->i--;
		return (err_unexpected_char(in));
	}

	if (not_hex_char(in->str[in->i]))
		return (err_unexpected_char(in));
	*n = get_hex_digit(in->str[in->i++]);

	while (1)
	{
		if (length_is_0(in) || not_hex_char(in->str[in->i]))
			return (0);

		*n = (*n << 4) | get_hex_digit(in->str[in->i++]);
		if (*n > 0xfffffffu)
			return (err_overflow(in));
	}
	return (0);
}

int		octal_ascii_to_uint(t_file *in, uint32_t *n)
{
	in->i++;
	if (length_is_0(in))
	{
		in->i--;
		return (err_unexpected_char(in));
	}

	if (in->str[in->i] == 'x')
		return (hexa_ascii_to_uint(in, n));
	
	if (not_octal_char(in->str[in->i]))
		return (err_unexpected_char(in));
	
	*n = (in->str[in->i] - '0');
	in->i++;
	while (1)
	{
		if (len_is_0(in) || not_octal_char(in->str[in->i]))
			return (0);

		*n = (*n << 3) | (in->str[in->i++] - '0');
		if (*n > 0xfffffffu)
			return (err_overflow(in));
	}
	return (0);
}

int		binary_ascii_to_uint(t_file *in, uint32_t *n)
{
	in->i++;
	if (length_is_0(in))
	{
		in->i--;
		return (err_unexpected_char(in));
	}

	if (not_binary_char(in->str[in->i]))
		return (err_unexpected_char(in));
	
	*n = (in->str[in->i] - '0');
	in->i++;
	while (1)
	{
		if (len_is_0(in))
			return (0);
		if (in->str[in->i] == '_')
		{
			in->i++;
			continue;
		}
		if (not_binary_char(in->str[in->i]))
			return (0);

		*n = (*n << 1) | (in->str[in->i++] - '0');
		if (*n > 0xfffffffu)
			return (err_overflow(in));
	}
	return (0);
}

int		decimal_ascii_to_uint(t_file *in, uint32_t *n)
{
	in->i++;
	if (length_is_0(in))
	{
		in->i--;
		return (err_unexpected_char(in));
	}

	if (not_decimal_char(in->str[in->i]))
		return (err_unexpected_char(in));
	
	*n = (in->str[in->i] - '0');
	in->i++;
	while (1)
	{
		if (len_is_0(in) || not_decimal_char(in->str[in->i]))
			return (0);

		*n = (*n * 10) + (in->str[in->i++] - '0');
		if (*n > 0xfffffffu)
			return (err_overflow(in));
	}
	return (0);
}