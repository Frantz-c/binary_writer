#include "interprete_file.h"

uint8_t		err_loop_argument(t_in *in)
{
	dprintf
	(
		STDERR_FILENO,
		"ERROR in file %s:l%u:\n"
		"\twrong syntax argument \"nlaps\"\n",
		in->name, in->line
	);
	return (1);
}

uint8_t		err_missing_quote(t_in *in)
{
	dprintf
	(
		STDERR_FILENO,
		"ERROR in file %s:l%u:\n"
		"\tmissing quote\n",
		in->name, in->line
	);
	return (1);
}

uint8_t		err_autopad_multi_stars(t_in *in)
{
	dprintf
	(
		STDERR_FILENO,
		"ERROR in file %s:l%u:\n"
		"\tmultiples '*' in auto-padding block\n",
		in->name, in->line
	);
	return (1);
}

uint8_t		err_missing_double_quote(t_in *in)
{
	dprintf
	(
		STDERR_FILENO,
		"ERROR in file %s:l%u:\n"
		"\tmissing double quote\n",
		in->name, in->line
	);
	return (1);
}

uint8_t		err_bs_x_no_digit(t_in *in)
{
	dprintf
	(
		STDERR_FILENO,
		"ERROR in file %s:l%u:\n"
		"\tno hexa digit after \\x\n",
		in->name, in->line
	);
	return (1);
}

uint8_t		err_unknown_balise(t_in *in)
{
	dprintf
	(
		STDERR_FILENO,
		"ERROR in file %s:l%u:\n"
		"\tunknown balise\n",
		in->name, in->line
	);
	return (1);
}

uint8_t		err_unexpected_char(t_in *in)
{
	if (in->str[in->i] > 0x7e || in->str[in->i] < 0x20)
	{
		dprintf(STDERR_FILENO,
				"ERROR in file %s:l%u:\n"
				"\tunexpected char '\\x%X'\n",
				in->name, in->line, in->str[in->i]
		);
	}
	else
	{
		dprintf(STDERR_FILENO,
				"ERROR in file %s:l%u:\n"
				"\tunexpected char '%c'\n",
				in->name, in->line, in->str[in->i]
		);
	}
	return (1);
}

uint8_t		err_overflow(t_in *in)
{
	dprintf(STDERR_FILENO,
			"ERROR in file %s:l%u:\n"
			"\toveflow\n",
			in->name, in->line
	);
	return (1);
}

uint8_t		err_missing_autopad_end(t_in *in, const uint32_t start_line)
{
	dprintf(STDERR_FILENO,
			"ERROR in file %s:l%u:\n"
			"\tno match with '[' line %u\n",
			in->name, in->line, start_line
	);
	return (1);
}

uint8_t		err_missing_parent_end(t_in *in,
								const uint32_t start_line)
{
	dprintf(STDERR_FILENO,
			"ERROR in file %s:l%u:\n"
			"\tno match with '(' line %u\n",
			in->name, in->line, start_line
	);
	return (1);
}

uint8_t		err_expected_char_at_end(t_in *in)
{
	dprintf(STDERR_FILENO,
			"ERROR in file %s:l%u:\n"
			"\tincomplete expression at EOL\n",
			in->name, in->line
	);
	return (1);
}


uint8_t		err_autopad_too_big_content(t_in *in)
{
	dprintf
	(
		STDERR_FILENO,
		"ERROR in file %s:l%u:\n"
		"\ttoo big auto-padding block content\n",
		in->name, in->line
	);
	return (1);
}

uint8_t		open_error(const char *const filename)
{
	dprintf
	(
		STDERR_FILENO,
		"can't open file \"%s\"\n",
		filename
	);
	return (1);
}

uint8_t		no_memory(const int fd)
{
	if (fd != -1)
		close(fd);
	dprintf
	(
		STDERR_FILENO,
		"ERROR: no more memory available: exit\n"
	);
	return (1);
}

uint8_t		empty_file(const char *const filename, const int fd)
{
	if (fd != -1)
		close(fd);
	dprintf
	(
		STDERR_FILENO,
		"ERROR: file \"%s\" is empty\n",
		filename
	);
	return (1);
}

uint8_t		lseek_error(const int fd)
{
	if (fd != -1)
		close(fd);
	dprintf
	(
		STDERR_FILENO,
		"ERROR: function lseek() has fail\n"
	);
	return (1);
}

uint8_t		read_error(const int fd, t_in *in)
{
	if (fd != -1)
		close(fd);
	free(in->str);
	dprintf
	(
		STDERR_FILENO,
		"ERROR: function read() has fail\n"
	);
	return (1);
}
