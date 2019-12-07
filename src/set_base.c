#include "interprete_file.h"

uint32_t	set_base(t_in *in, t_out *out, t_data *data, t_ustr *buf)
{
	// skip '<'
	in->i++;

	if (length_ok(in, 5) && memcmp(in->str + in->i, "hexa>", 5) == 0)
	{
		data->base = HEXA_BASE;
		in->i += 5;
	}
	else if (length_ok(in, 4) && memcmp(in->str + in->i, "bin>", 4) == 0)
	{
		data->base = BINARY_BASE;
		in->i += 4;
	}
	else if (length_ok(in, 7) && memcmp(in->str + in->i, "binary>", 7) == 0)
	{
		data->base = BINARY_BASE;
		in->i += 7;
	}
	else if (length_ok(in, 4) && memcmp(in->str + in->i, "oct>", 4) == 0)
	{
		data->base = OCTAL_BASE;
		in->i += 4;
	}
	else if (length_ok(in, 6) && memcmp(in->str + in->i, "octal>", 6) == 0)
	{
		data->base = OCTAL_BASE;
		in->i += 6;
	}
	else if (length_ok(in, 2) && memcmp(in->str + in->i, "*>", 2) == 0)
	{
		data->base = ALL_BASES;
		in->i += 2;
	}
	else if (length_ok(in, 4) && memcmp(in->str + in->i, "all>", 4) == 0)
	{
		data->base = ALL_BASES;
		in->i += 4;
	}
	else
	{
		skip_word(in);
		return (err_unknown_balise(in));
	}
	return (0);
}
