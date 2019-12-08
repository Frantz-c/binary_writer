#include "interpret_file.h"

// loop syntax: "().n"
static uint8_t		get_laps(t_in *in, uint32_t	*nlaps)
{
	// skip ')'
	in->i++;

	if (len_is_0(in))
		return (err_loop_argument(in));

	if (in->str[in->i] != '.')
		return (err_loop_argument(in));


	in->i++;
	if (len_is_0(in))
		return (err_loop_argument(in));

	if (get_number(in, nlaps) == 1)
		return (1);
	if (len_is_not_0(in)
			&& in->str[in->i] != ' '
			&& in->str[in->i] != '\t'
			&& in->str[in->i] != '\n'
			&& in->str[in->i] != '\r')
	{
		return (err_unexpected_char(in));
	}

	return (0);
}

static void		copy_unrolled_loop(t_out *out, t_ustr *buf, t_data *data)
{
	register const uint32_t	nlaps = data->param1;

	if (buf->str == NULL)
		return;

	if (data->loop_lvl > 1 || data->autopad)
	{
		if (buf->len < (buf->i * nlaps))
		{
			buf->str = realloc(buf->str, buf->i * nlaps);
			buf->len = buf->i * nlaps;
		}
		for (uint32_t a = 1, b = buf->i; a < nlaps; a++, b += buf->i)
		{
			memmove(buf->str + b, buf->str, buf->i);
		}
		buf->i *= nlaps;
	}
	else // if loop isn't called by autopad and loop_lvl == 1
	{
		if ((out->len - out->i) < (buf->i * nlaps))
		{
			out->str = realloc(out->str, out->len + (buf->i * nlaps));
			out->len += (buf->i * nlaps);
		}
		for (uint32_t a = 0; a < nlaps; a++)
		{
			memcpy(out->str + out->i, buf->str, buf->i);
			out->i += buf->i;
		}
		buf->i = 0;
	}
}

// create a static struct
uint32_t	get_loop(t_in *in, t_out *out, t_data *data, t_ustr *buf)
{
	const uint32_t		start_line = in->line;
	uint32_t			error = 0;

	// skip '('
	in->i++;

	data->loop_lvl++;
//	dprintf(2, "loop_%u.content = [ %c ]\n",
//			data->loop_lvl, in->str[in->i]);

	// read the loop content
	while (in->i < in->len)
	{
		// skip spaces
		while (1)
		{
			if (in->str[in->i] == '\n')
				in->line++;
			else if (in->str[in->i] != ' ' && in->str[in->i] != '\t')
				break;
			in->i++;
			if (in->i == in->len)
			{
				data->loop_lvl--;
				return (error + err_missing_parent_end(in, start_line));
			}
		}

		if (in->str[in->i] == ')')
		{
			error += get_laps(in, &data->param1);
			if (!error)
				copy_unrolled_loop(out, buf, data);
			else
				buf->i = 0;
			data->loop_lvl--;
//			dprintf(2, "(2) loop_%u.content = [ %.*s ]\n",
//				data->loop_lvl, (int)(in->len - in->i), in->str + in->i);
			return (error);
		}
		else
		{
			uint8_t	chr = in->str[in->i];
			uint8_t	index = get_func_index[data->base][chr];

			if (index == 0xFFu)
			{
				error += err_unexpected_char(in);
				skip_until_match(in, ')');
				data->loop_lvl--;
				return (error + 1);
			}
			else
				error += call[index](in, out, data, buf);
		}
		in->i++;
	}

	__error_loop:
	dprintf(STDERR_FILENO,
			"file %s:l%u:\terror syntax in loop\n",
			in->name, in->line
	);
	data->loop_lvl--;
	return (error + 1);
}
