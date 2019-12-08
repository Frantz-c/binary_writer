#include "interpret_file.h"

uint32_t	get_octal(t_in *in, t_out *out, t_data *data, t_ustr *buf)
{
	uint32_t	n;

	if (in->str[in->i] == 'o')
		in->i++;
	else if (in->str[in->i] == '0')
	{
		in->i++;
		if (len_is_0(in))
		{
			write_a_char(out, buf, 0u, data);
			return (0);
		}
		if (in->str[in->i] == 'x')
			return (get_hexa(in, out, data, buf));
	}

	if (octal_ascii_to_uint(in, &n) == 1)
	{
		skip_word(in);
		return (1);
	}
	if (n > 0xFFu)
		return (err_overflow(in));
	write_a_char(out, buf, (uint8_t)n, data);

	return (0);
}
