#include "interprete_file.h"

uint32_t	skip_comment(t_in *in, t_out *out, t_data *data, t_ustr *buf)
{
	in->i++;
	while (len_is_not_0(in) && in->str[in->i] != '\n')
		in->i++;
	return (0);
}
