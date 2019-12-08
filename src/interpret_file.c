#include "interpret_file.h"

static int	get_file_content(t_in *in, const char *filename)
{
	int	fd;

	// OPEN
	fd = open(filename, O_RDONLY);
	if (fd == -1)
		return (open_error(filename));

	// LSEEK: Get file size
	in->len = (uint32_t)lseek(fd, 0, SEEK_END);
	if (in->len & 0x80000000u)
		return (lseek_error(fd));
	if (in->len == 0)
		return (empty_file(filename, fd));
	lseek(fd, 0, SEEK_SET);

	// allocate buffer to copy the file
	in->str = malloc(in->len);
	if (in->str == NULL)
		return (no_memory(fd));

	// copy the file
	if (read(fd, in->str, in->len) != in->len)
		return (read_error(fd, in));

	close(fd);
	return (0);
}

int		interpret_file(const char *filename)
{
	t_in		in = {NULL};
	t_out		out = {NULL};
	t_data		data = {0};
	t_ustr		buf = {NULL};
	uint32_t	error = 0;

	if (get_file_content(&in, filename) != 0)
		return (1);

	in.line = 1;
	in.name = filename;
	out.name = NULL;


	while (in.i < in.len)
	{
		// skip spaces
		while (1)
		{
			if (in.str[in.i] == '\n')
				in.line++;
			else if (in.str[in.i] != ' ' && in.str[in.i] != '\t')
				break;
			in.i++;
			if (in.i == in.len)
				goto __end_loops;
		}

		// using character to get function index and call it
		{
			uint8_t	chr = in.str[in.i];
			uint8_t	index = get_func_index[data.base][chr];

			if (index == 0xffu)
				error += err_unexpected_char(&in);
			else
				call[index](&in, &out, &data, &buf);
		}
		in.i++;
	}

	__end_loops:
	if (out.str == NULL)
	{
		dprintf(STDERR_FILENO, "ERROR: not a bwl file\n");
		free(in.str);
		return (1);
	}
	if (!error)
		write(STDOUT_FILENO, out.str, out.i);
	free(out.str);
	free(in.str);

	return (0);
}
