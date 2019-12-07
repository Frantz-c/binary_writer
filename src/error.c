#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdint.h>
#include <ctype.h>
#include <fcntl.h>

uint8_t		err_unexpected_char(t_file *in)
{
	if (in->str[in->i] > 0x7e || in->str[in->i] < 0x20)
	{
		dprintf(STDERR_FILENO,
				"file %s:l%u:\tunexpected char '\\x%X'\n",
				in->name, in->line, in->str[in->i]
		);
	}
	else
	{
		dprintf(STDERR_FILENO,
				"file %s:l%u:\tunexpected char '%c'\n",
				in->name, in->line, in->str[in->i]
		);
	}
	return (1);
}

uint8_t		err_overflow(t_file *in)
{
	dprintf(STDERR_FILENO,
			"file %s:l%u:\tnumber oveflow\n",
			in->name, in->line
	);
	return (1);
}

uint8_t		err_missing_autopad_end(t_file *in, const uint32_t start_line)
{
	dprintf(STDERR_FILENO,
			"file %s:l%u:\tno match with '[' line %u\n",
			in->name, in->line, start_line
	);
	return (1);
}

uint8_t		err_expected_char_at_end(t_file *in)
{
	dprintf(STDERR_FILENO,
			"file %s:l%u:\tincomplete expression at EOL\n",
			in->name, in->line
	);
	return (1);
}
