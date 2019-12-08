#include "interpret_file.h"

uint32_t	get_hexa(t_in *in, t_out *out, t_data *data, t_ustr *buf)
{
	uint32_t	n;

	if (in->str[in->i] == 'h' || in->str[in->i] == 'x')
		in->i++;

	if (hexa_ascii_to_uint(in, &n) == 1)
	{
		skip_word(in);
		return (1);
	}
	if (n > 0xFFu)
		return (err_overflow(in));
	write_a_char(out, buf, (uint8_t)n, data);

	return (0);
}
