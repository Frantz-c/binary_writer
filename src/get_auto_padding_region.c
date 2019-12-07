#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdint.h>
#include <ctype.h>
#include <fcntl.h>

#define PAD_BEG	1
#define PAD_END	2
#define PAD_MID	4

// coder skip_until_match()


/*
*	parse the auto-padding block arguments:
*		".n,p"	or	".n,*=p"
*/
uint8_t get_autopad_info(t_file *in, uint8_t *pad, uint32_t *size)
{
	in->i++;
	if (length_is_0(in))
		return (err_expected_char_at_end(in));

	if (in->str[in->i] != '.')
		return (err_unexpected_char(in));
	
	in->i++;
	if (length_is_0(in))
		return (err_expected_char_at_end(in));

	get_number(in, size);
	if (length_is_0(in))
		return (err_expected_char_at_end(in));
	if (in->str[in->i] != ',')
		return (err_unexpected_char(in));

	in->i++;
	if (length_is_0(in))
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

void	write_autopad_block(t_file *out)
{
	
}

uint8_t	get_auto_padding_block(t_file *in, t_file *out)
{
	const uint32_t	start_line	= in->line;
	uint8_t			error		= 0;
	uint8_t			pad_pos		= 0;

	in->i++;
	if (length_is_0(in))
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
			if (in->i == i->len)
				return (err_missing_autopad_end(in, start_line));
		}

		if (length_is_0(in))
			return (err_missing_autopad_end(in, start_line));

		if (in->str[in->i] == ']')
		{
			uint8_t		pad;
			uint32_t	size;
			error += get_autopad_info(in, &pad, &size);
			if (!error)
				write_autopad_block(out);
			return (error);
		}
		else if (in->str[in->i] == '*')
		{
			if ()
		}
		else if (router[in->str[in->i]] == NULL)
		{
			error += err_unexpected_char(in);
			skip_until_match(in, ']');
			return (error);
		}
		else
			error += router[in->str[in->i]](&in, &out);
	}
	return (0);
}
