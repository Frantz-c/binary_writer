#include "interprete_file.h"
/*
static void		write_a_char(t_out *out, uint8_t chr)
{
	if (len_is_0(out))
	{
		out->str = realloc(out->str, out->len + 0x40u);
		out->len += 0x40u;
	}
	out->str[out->i++] = chr;
}
*/
static uint8_t		write_escape_sequence(t_in *in, t_out *out, uint32_t len)
{
	uint32_t	chr;

	if (in->str[in->i] == 'x') // hexa
	{
		in->i++;
		if (!--len)
			return (0);
		if (not_hex_char(in->str[in->i]))
			return (err_bs_x_no_digit(in));
		in->i--;		// not good
		hexa_ascii_to_uint(in, &chr);
		if (chr > 0xFFu)
			return (err_overflow(in));
		write_a_char(out, (uint8_t)chr);
	}
	else if (is_octal_char(in->str[in->i])) // octal
	{
		in->i++;
		if (!--len)
			return (0);
		in->i--;	// not good
		octal_ascii_to_uint(in, &chr);
		if (chr > 0xFFu)
			return (err_overflow(in));
		write_a_char(out, (uint8_t)chr);
	}
	else if (in->str[in->i] == 'e')
		write_a_char(out, '\e');
	else if (in->str[in->i] == 'n')
		write_a_char(out, '\n');
	else if (in->str[in->i] == 'r')
		write_a_char(out, '\r');
	else if (in->str[in->i] == 'a')
		write_a_char(out, '\a');
	else if (in->str[in->i] == 'b')
		write_a_char(out, '\b');
	else if (in->str[in->i] == 'f')
		write_a_char(out, '\f');
	else if (in->str[in->i] == 'v')
		write_a_char(out, '\v');
	else
		return (0);
	return (0);
}

uint32_t	get_string(t_in *in, t_out *out, t_data *data, t_ustr *buf)
{
	uint32_t	len;
	// skip double quote
	in->i++;
	len = in->len - in->i;

	if (!len)
		return (err_missing_double_quote(in));

	while (in->str[in->i] != '"')
	{
		if (in->str[in->i] < 0x32)
			return (err_unexpected_char(in));
		if (in->str[in->i] == '\\')
		{
			in->i++;
			if (len_is_0(in))
				return (err_missing_double_quote(in));
			write_escape_sequence(in, out, len);
			len = in->len - in->i;
		}
		else
			write_a_char(out, in->str[in->i]);

		if (!len)
			return (err_missing_double_quote(in));

		in->i++;
		len--;
	}
	return (0);
}
