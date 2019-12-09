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

	return (0);
}

static void		copy_unrolled_loop(t_out *out, t_ustr *buf, t_data *data, uint32_t buf_start)
{
	register const uint32_t	nlaps = data->param1;

	if (buf->str == NULL)
		return;

	if (data->loop_lvl > 1 || data->autopad)
	{
		if (buf->len < buf_start + ((buf->i - buf_start) * nlaps))
		{
			buf->str = realloc(buf->str, (buf_start + (buf->i - buf_start)) * nlaps);
			buf->len = buf_start + ((buf->i - buf_start) * nlaps);
		}
		for (uint32_t a = 1, b = (buf->i - buf_start); a < nlaps; a++, b += (buf->i - buf_start))
		{
			memmove(buf->str + buf_start + b, buf->str + buf_start, (buf->i - buf_start));
		}
		buf->i = buf_start + ((buf->i - buf_start) * nlaps);
	}
	else // if loop isn't called by autopad and loop_lvl == 1
	{
		if ((out->len - out->i) < buf_start + ((buf->i - buf_start) * nlaps))
		{
			out->str = realloc(out->str, out->len + ((buf->i - buf_start) * nlaps));
			out->len += ((buf->i - buf_start) * nlaps);
		}
		for (uint32_t a = 0; a < nlaps; a++)
		{
			memcpy(out->str + out->i, buf->str, (buf->i - buf_start));
			out->i += (buf->i - buf_start);
		}
		buf->i = 0;
	}
}

// create a static struct
uint32_t	get_loop(t_in *in, t_out *out, t_data *data, t_ustr *buf)
{
	const uint32_t		start_line = in->line;
	uint32_t			buf_start = buf->i;
	uint32_t			error = 0;

	// skip '('
	in->i++;

	data->loop_lvl++;

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
				copy_unrolled_loop(out, buf, data, buf_start);
			else
				buf->i = 0;
			data->loop_lvl--;
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
			{
				error += call[index](in, out, data, buf);
				continue ;
			}
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
