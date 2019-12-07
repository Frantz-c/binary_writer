#include "interprete_file.h"

extern void		write_a_char(t_out *out, uint8_t chr)
{
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
