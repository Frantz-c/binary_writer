#include "interprete_file.h"

#define PAD_BEG	1
#define PAD_END	2
#define PAD_MID	4

/*
*	parse auto-padding block arguments:
*		".n,p"	or	".n,*=p"
*/
static uint8_t get_autopad_info(t_in *in, uint8_t *pad, uint32_t *size)
{
	in->i++;
	if (len_is_0(in))
		return (err_expected_char_at_end(in));

	if (in->str[in->i] != '.')
		return (err_unexpected_char(in));
	
	in->i++;
	if (len_is_0(in))
		return (err_expected_char_at_end(in));

	if (get_number(in, size) == 1)
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
	*pad = in->str[in->i];
	return (0);
}

static uint8_t	write_autopad_block(t_in *in, t_out *out, t_ustr *buf,
							uint8_t pad_pos, uint8_t pad_chr,
							uint32_t block_size, uint32_t prefix_size)
{
	uint32_t pad_size;

	if (pad_pos == PAD_END && buf->i)
	{
		pad_size = (block_size - (prefix_size + buf->i));
		if (pad_size & 0x80000000)
			return (err_autopad_too_big_content(in));
		if (length_not_ok(out, buf->i + pad_size))
		{
			out->str = realloc(out->str, out->len + buf->i + pad_size);
			out->len += buf->i + pad_size;
		}
		memset(out->str + out->i, pad_chr, pad_size);
		out->i += pad_size;
		memcpy(out->str + out->i, buf->str, buf->i);
		out->i += buf->i;
		return (0);
	}
	else if (pad_pos == PAD_BEG)
	{
		pad_size = (block_size - buf->i);
		if (pad_size & 0x80000000)
			return (err_autopad_too_big_content(in));
		if (length_not_ok(out, block_size))
		{
			out->str = realloc(out->str, block_size);
			out->len += block_size;
		}
		memset(out->str + out->i, pad_chr, pad_size);
		out->i += pad_size;
		memcpy(out->str + out->i, buf->str, buf->i);
		out->i += buf->i;
	}
	else
	{
		pad_size = (block_size - buf->i);
		if (pad_size & 0x80000000)
			return (err_autopad_too_big_content(in));
		if (length_not_ok(out, block_size))
		{
			out->str = realloc(out->str, block_size);
			out->len += block_size;
		}
		memcpy(out->str + out->i, buf->str, buf->i);
		out->i += buf->i;
		memset(out->str + out->i, pad_chr, pad_size);
		out->i += pad_size;
	}
	return (0);
}

uint32_t	get_autopad(t_in *in, t_out *out, t_data *data, t_ustr *buf)
{
	const uint32_t	start_line	= in->line;
	uint8_t			error		= 0;
	uint8_t			pad_pos		= 0;
	uint32_t		prefix_size	= 0;

	in->i++;
	if (len_is_0(in))
	{
		in->i--;
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
				return (error + err_missing_autopad_end(in, start_line));
		}

		if (in->str[in->i] == ']')
		{
			uint8_t		pad_chr;
			uint32_t	size;
			error += get_autopad_info(in, &pad_chr, &size);
			if (!error)
				write_autopad_block(in, out, buf, pad_pos, pad_chr, size, prefix_size);
			else
				buf->i = 0;
			return (error);
		}
		else if (in->str[in->i] == '*')
		{
			if (pad_pos)
				return (error + err_autopad_multi_stars(in));
			if (buf->i == 0)
				pad_pos = PAD_BEG;
			else
			{
				prefix_size = buf->i;
				if (length_not_ok(out, buf->i))
				{
					out->str = realloc(out->str, out->len + buf->i);
					out->len += buf->i;
				}
				memcpy(out->str + out->i, buf->str, buf->i);
				out->i += buf->i;
				pad_pos = PAD_END;
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
				return (error);
			}
			error += call[index](in, out, data, buf);
		}
	}
	return (error);
}
