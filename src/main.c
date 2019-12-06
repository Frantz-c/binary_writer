#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdint.h>
#include <ctype.h>

void	print_help(const char *const prog)
{
	printf("%s [file.bwl]        -> generate binary using BWL file\n"
			"%s	--convert [file]  -> convert binary to BWL\n"
			"%s                   -> direct input mode\n"
			"%s	-h                -> display help\n"
			"%s	--bwl             -> display bwl syntax help\n",
			prog, prog, prog, prog, prog
	);
}

uint8_t	get_program_arguments(int ac, char *av[], char **in)
{
	uint8_t	mode = 0;

	if (ac == 0)
		return (INPUT_MODE);

	if (memcmp("--convert", av[0], 10) == 0)
	{
		mode = CONVERT_MODE;
		ac--;
		av++;
	}
	else if (memcmp("-h", av[0], 3) == 0 ||
			memcmp("--help", av[0], 7) == 0)
	{
		return (HELP_MODE);
	}
	else
		mode = INTERPRETE_MODE;
	
	if (ac != 1)
		return (HELP_MODE);

	*in = *av;
	return (mode);
}

int		main(int argc, char *argv[])
{
	uint8_t	mode;
	char	*in;

	if (argc == 0)
		return (2);
	mode = get_program_arguments(argc - 1, argv + 1, &in);

	switch (mode)
	{
		case HELP_MODE:
			print_help(*av);
			return (0);
		case INTERPRETE_MODE:
			return (interprete_file(in));
		case CONVERT_MODE:
			return (convert_file(in));
		case INPUT_MODE:
			return (direct_input());
	}
}
