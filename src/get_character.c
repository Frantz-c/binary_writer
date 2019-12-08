#include "interpret_file.h"

uint32_t	get_character(t_in *in, t_out *out, t_data *data, t_ustr *buf)
{
	// skip quote
	in->i++;

	if (len_is_0(in))
		return (err_missing_quote(in));

	if (in->str[in->i] < 0x20)
		return (err_unexpected_char(in));
	if (in->str[in->i] == '\\')
	{
		in->i++;
		if (len_is_0(in))
			return (err_missing_quote(in));
		write_escape_sequence(in, out, buf, data);
	}
	else
		write_a_char(out, buf, in->str[in->i], data);
	in->i++;

	if (len_is_0(in) || in->str[in->i] != '\'')
		return (err_missing_quote(in));
	return (0);
}
