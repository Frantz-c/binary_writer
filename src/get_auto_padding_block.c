#include "interpret_file.h"

/*
*	parse auto-padding block arguments:
*		".n,p"	or	".n,*=p"
*/
static uint8_t get_autopad_info(t_in *in, t_data *data)
{
	uint32_t	pad_chr;

	in->i++;
	if (len_is_0(in))
		return (err_expected_char_at_end(in));

	if (in->str[in->i] != '.')
		return (err_unexpected_char(in));
	
	in->i++;
	if (len_is_0(in))
		return (err_expected_char_at_end(in));

	if (get_number(in, &data->param1) == 1)
		return (1);
	if (len_is_0(in))
		return (err_expected_char_at_end(in));
	if (in->str[in->i] != ',')
		return (err_unexpected_char(in));

	in->i++;
	if (len_is_0(in))
		return (err_expected_char_at_end(in));
	
	if (in->str[in->i] == '*')
	{
		in->i++;
		if (length_not_ok(in, 2))
			return (err_expected_char_at_end(in));
		if (in->str[in->i] != '=')
			return (err_unexpected_char(in));
		in->i++;
	}
	if (get_number(in, &pad_chr) == 1)
		return (1);
	data->param2 = (uint8_t)pad_chr;
	return (0);
}

static uint8_t	write_autopad_block(t_in *in, t_out *out,
									t_data *data, t_ustr *buf)
{
	uint32_t pad_size;
	// data->param2 is padding_character
	// data->param1 is blocksize

	if (data->pad_pos == PAD_END && buf->i)
	{
		pad_size = (data->param1 - (data->pre_size + buf->i));
		if (pad_size & 0x80000000u)
			return (err_autopad_too_big_content(in));
		if (length_not_ok(out, buf->i + pad_size))
		{
			out->str = realloc(out->str, out->len + buf->i + pad_size);
			out->len += buf->i + pad_size;
		}
		memset(out->str + out->i, data->param2, pad_size);
		out->i += pad_size;
		memcpy(out->str + out->i, buf->str, buf->i);
		out->i += buf->i;
	}
	else if (data->pad_pos == PAD_BEG)
	{
		pad_size = (data->param1 - buf->i);
		if (pad_size & 0x80000000u)
			return (err_autopad_too_big_content(in));
		if (length_not_ok(out, (pad_size + buf->i)))
		{
			out->str = realloc(out->str, out->len + (pad_size + buf->i));
			out->len += (pad_size + buf->i);
		}
		memset(out->str + out->i, data->param2, pad_size);
		out->i += pad_size;
		memcpy(out->str + out->i, buf->str, buf->i);
		out->i += buf->i;
	}
	else
	{
		pad_size = (data->param1 - data->pre_size);
		if (pad_size & 0x80000000u)
			return (err_autopad_too_big_content(in));
		if (length_not_ok(out, pad_size))
		{
			out->str = realloc(out->str, out->len + pad_size);
			out->len += pad_size;
		}
		memset(out->str + out->i, data->param2, pad_size);
		out->i += pad_size;
	}
	buf->i = 0;
	return (0);
}

uint32_t	get_autopad(t_in *in, t_out *out, t_data *data, t_ustr *buf)
{
	const uint32_t	start_line	= in->line;
	uint8_t			error		= 0;

	data->autopad++;
	data->pre_size = 0;
	data->pad_pos = 0;
	data->param1 = 0;
	data->param2 = 0;

	in->i++;
	if (len_is_0(in))
	{
		in->i--;
		data->autopad--;
		return (err_unexpected_char(in));
	}

	while (1)
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
				data->autopad--;
				return (error + err_missing_autopad_end(in, start_line));
			}
		}

		if (in->str[in->i] == ']')
		{
			error += get_autopad_info(in, data);
			if (!error)
				write_autopad_block(in, out, data, buf);
			else
				buf->i = 0;
			data->autopad--;
			return (error);
		}
		else if (in->str[in->i] == '*')
		{
			if (data->pad_pos)
			{
				data->autopad--;
				return (error + err_autopad_multi_stars(in));
			}
			if (buf->i == 0)
				data->pad_pos = PAD_BEG;
			else
			{
				data->pre_size = buf->i;
				if (length_not_ok(out, buf->i))
				{
					out->str = realloc(out->str, out->len + buf->i);
					out->len += buf->i;
				}
				memcpy(out->str + out->i, buf->str, buf->i);
				out->i += buf->i;
				buf->i = 0;
				data->pad_pos = PAD_END;
			}
		}
		else
		{
			uint8_t	chr = in->str[in->i];
			uint8_t	index = get_func_index[data->base][chr];

			if (index == 0xFFu)
			{
				error += err_unexpected_char(in);
				skip_until_match(in, ']');
				data->autopad--;
				return (error);
			}
			error += call[index](in, out, data, buf);
		}
		in->i++;
		if (len_is_0(in))
		{
			data->autopad--;
			return (error + err_autopad_multi_stars(in));
		}
	}
	data->autopad--;
	return (error);
}
