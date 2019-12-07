#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdint.h>
#include <ctype.h>
#include <fcntl.h>

// create get_number function

void		get_laps_and_duplicate_n_times(t_file *in, t_file *out)
{
	uint32_t	n_laps;

	in->i++;

	if (len_is_0(in))
		goto __error_end_loop;

	if (in->str[in->i] != '.')
		goto __error_syntax_n_laps;


	in->i++;
	if (len_is_0(in))
		goto __error_end_loop;

	if (get_number(in, &n_laps) == -1)
		return;
	if (length_is_not_0(in)
			&& in->str[in->i] != ' '
			&& in->str[in->i] != '\t'
			&& in->str[in->i] != '\n'
			&& in->str[in->i] != '\r')
		return (err_unexpected_char(in));


	if (!in->buf)
		return;

	if (in->parent_lvl != 1)
	{
		if ((out->len - out->i) < (out->j * n_laps))
			out->buf = realloc(out->buf, out->j * n_laps);
		for (uint32_t a = 0, b = out->j; a < n_laps; a++, b += out->j)
			memcpy(out->buf + b, out->buf, out->buflen);
		out->buflen *= n_laps;
	}
	else
	{
		if ((out->len - out->i) < (out->j * n_laps))
			out->str = realloc(out->str, out->len + (out->j * n_laps));
		for (uint32_t a = 0; a < n_laps; a++)
		{
			memcpy(out->str + out->i, out->buf, out->j);
			out->i += out->j;
		}
		out->len += (out->j * n_laps);
	}
}

uint8_t		get_loop(t_file *in, t_file *out)
{
	in->i++;
	in->parent_lvl++;

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
			if (in->i == i->len)
				goto __error_loop;
		}

		if (in->str[in->i] == ')')
		{
			get_laps_and_duplicate_n_times(&in, &out);
			in->parent_lvl--;
			return (0);
		}
		if (in->str[in->i] > 0x80 || router[in->str[in->i]] == NULL)
		{
			dprintf(
				STDERR_FILENO,
				"file %s:l%u:\tunexpected character '%c'\n",
				in->name, in->line, in->str[in->i]
			);
		}
		else
			error += router[in->str[in->i]](&in, &out);
	}

	__error_loop:
	dprintf(STDERR_FILENO,
			"file %s:l%u:\terror syntax in loop\n",
			in->name, in->line
	);
	return (1);
}
