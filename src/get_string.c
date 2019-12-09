#include "interpret_file.h"

uint32_t	get_string(t_in *in, t_out *out, t_data *data, t_ustr *buf)
{
	// skip double quote
	in->i++;

	if (len_is_0(in))
		return (err_missing_double_quote(in));

	while (in->str[in->i] != '"')
	{
		if (in->str[in->i] < 0x20)
			return (err_unexpected_char(in));
		if (in->str[in->i] == '\\')
		{
			in->i++;
			if (len_is_0(in))
				return (err_missing_double_quote(in));
			write_escape_sequence(in, out, buf, data);
		}
		else
			write_a_char(out, buf, in->str[in->i], data);

		if (len_is_0(in))
			return (err_missing_double_quote(in));

		in->i++;
	}
	in->i++;
	return (0);
}
