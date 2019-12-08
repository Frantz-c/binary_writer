#include "interpret_file.h"

extern  uint8_t		write_escape_sequence(t_in *in, t_out *out,
										t_ustr *buf, t_data *data)
{
	uint32_t	chr;

	if (in->str[in->i] == 'x') // hexa
	{
		in->i++;
		if (len_is_0(in))
			return (0);
		if (not_hex_char(in->str[in->i]))
			return (err_bs_x_no_digit(in));
		hexa_ascii_to_uint(in, &chr);
		in->i--;
		if (chr > 0xFFu)
			return (err_overflow(in));
		write_a_char(out, buf, (uint8_t)chr, data);
	}
	else if (is_octal_char(in->str[in->i])) // octal
	{
		octal_ascii_to_uint(in, &chr);
		in->i--;
		if (chr > 0xFFu)
			return (err_overflow(in));
		write_a_char(out, buf, (uint8_t)chr, data);
	}
	else if (in->str[in->i] == 'e')
		write_a_char(out, buf, '\e', data);
	else if (in->str[in->i] == 'n')
		write_a_char(out, buf, '\n', data);
	else if (in->str[in->i] == 'r')
		write_a_char(out, buf, '\r', data);
	else if (in->str[in->i] == 'a')
		write_a_char(out, buf, '\a', data);
	else if (in->str[in->i] == 'b')
		write_a_char(out, buf, '\b', data);
	else if (in->str[in->i] == 'f')
		write_a_char(out, buf, '\f', data);
	else if (in->str[in->i] == 'v')
		write_a_char(out, buf, '\v', data);
	else
		in->i++;
	return (0);
}

extern void	write_a_char(t_out *out, t_ustr *buf, uint8_t chr, t_data *data)
{
	if (data->loop_lvl || data->autopad)
	{
		if (len_is_0(buf))
		{
			buf->str = realloc(buf->str, buf->len + 0x40u);
			buf->len += 0x40u;
		}
		buf->str[buf->i++] = chr;
		return;
	}

	if (len_is_0(out))
	{
		out->str = realloc(out->str, out->len + 0x40u);
		out->len += 0x40u;
	}
	out->str[out->i++] = chr;
}

extern void		skip_word(t_in *in)
{
	while (len_is_not_0(in)
			&& (in->str[in->i] != ' ' &&
				in->str[in->i] != '\t' &&
				in->str[in->i] != '\n'))
		in->i++;
}

extern void		skip_until_match(t_in *in, uint8_t chr)
{
	while (len_is_not_0(in) && in->str[in->i] != chr)
		in->i++;
}
