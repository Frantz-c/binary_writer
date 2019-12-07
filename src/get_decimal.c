#include "interprete_file.h"

uint32_t	get_decimal(t_in *in, t_out *out, t_data *data, t_ustr *buf)
{
	uint32_t	n;

	if (decimal_ascii_to_uint(in, &n) == 1)
	{
		skip_word(in);
		return (1);
	}
	if (n > 0xFFu)
		return (err_overflow(in));
	write_a_char(out, (uint8_t)n);
	return (0);
}
