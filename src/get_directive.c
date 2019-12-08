#include "interpret_file.h"

uint32_t	get_directive(t_in *in, t_out *out, t_data *data, t_ustr *buf)
{
	in->i++;
	dprintf(STDERR_FILENO, "%%include and %%macro not implemented\n");
	return (1);
}
