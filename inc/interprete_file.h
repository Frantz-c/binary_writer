#ifndef INTERPRETE_FILE_H
# define INTERPRETE_FILE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdint.h>
#include <ctype.h>
#include <fcntl.h>

# define I_STRING				0
# define I_COMMENT				1
# define I_DIRECTIVE			2
# define I_CHAR					3
# define I_LOOP					4
# define I_B8					5
# define I_B10					6
# define I_B16					7
# define I_B2					8
# define I_BASE					9
# define I_AUTOPAD				10
# define I_END					11

#define is_binary_char(c)\
	(c == '1' || c == '0')

#define not_binary_char(c)\
	(c != '1' && c != '0')

#define is_octal_char(c)\
	(c >= '0' && c <= '7')

#define not_octal_char(c)\
	(c < '0' || c > '7')

#define is_decimal_char(c)\
	(c >= '0' && c <= '9')

#define not_decimal_char(c)\
	(c < '0' || c > '9')

#define is_hex_char(c)\
	((c >= 'a' && c <= 'f') || (c >= 'A' && c <= 'F')\
	|| (c >= '0' && c <= '9'))

#define	not_hex_char(c)\
	(c > 'f' || (c < 'a' && c > 'F')\
	|| (c < 'A' && c > '9') || c < '0')

# define length_ok(file,n)		((file->len - file->i) >= n)
# define length_not_ok(file,n)	((file->len - file->i) < n)
# define len_is_0(file)			(file->i >= file->len)
# define len_is_not_0(file)		(file->i < file->len)

# define ALL_BASES		0x0u
# define HEXA_BASE		0x1u
# define OCTAL_BASE		0x2u
# define BINARY_BASE	0x3u
# define TOTAL_BASES	0x4u

typedef struct	s_in
{
	uint8_t		*str;
	uint32_t	len;
	uint32_t	i;
	uint32_t	line;
	const char	*name;
}
t_in;

typedef struct	s_data
{
	uint8_t		base;
	uint8_t		autopad;
	uint8_t		loop_lvl;
}
t_data;

typedef struct	s_ustring
{
	uint8_t		*str;
	uint32_t	len;
	uint32_t	i;
}
t_ustr;

typedef struct	s_out
{
	uint8_t		*str;
	uint32_t	len;
	uint32_t	i;
	const char	*name;
}
t_out;

extern uint8_t	get_func_index[TOTAL_BASES][0x100];
extern uint32_t	(*call[I_END])(t_in*,t_out*,t_data*,t_ustr*);

// functions.c
void		write_a_char(t_out *out, uint8_t chr);
void		skip_word(t_in *in);
void		skip_until_match(t_in *in, uint8_t chr);

// error.c
uint8_t		err_loop_argument(t_in *in);
uint8_t		err_missing_quote(t_in *in);
uint8_t		err_missing_double_quote(t_in *in);
uint8_t		err_autopad_multi_stars(t_in *in);
uint8_t		err_bs_x_no_digit(t_in *in);
uint8_t		err_unknown_balise(t_in *in);
uint8_t		err_unexpected_char(t_in *in);
uint8_t		err_overflow(t_in *in);
uint8_t		err_missing_autopad_end(t_in *in,
								const uint32_t start_line);
uint8_t		err_missing_parent_end(t_in *in,
								const uint32_t start_line);
uint8_t		err_expected_char_at_end(t_in *in);
uint8_t		err_autopad_too_big_content(t_in *in);
uint8_t		open_error(const char *const filename);
uint8_t		no_memory(const int fd);
uint8_t		empty_file(const char *const filename,
						const int fd);
uint8_t		lseek_error(const int fd);
uint8_t		read_error(const int fd, t_in *in);

// ascii_base_to_uint.c
uint8_t		hexa_ascii_to_uint(t_in *in, uint32_t *n);
uint8_t		octal_ascii_to_uint(t_in *in, uint32_t *n);
uint8_t		binary_ascii_to_uint(t_in *in, uint32_t *n);
uint8_t		decimal_ascii_to_uint(t_in *in, uint32_t *n);

// get_auto_padding_block.c
uint32_t	get_autopad(t_in *in, t_out *out, t_data *data, t_ustr *buf);

// get_binary.c
uint32_t	get_binary(t_in *in, t_out *out, t_data *data, t_ustr *buf);

// get_character.c
uint32_t	get_character(t_in *in, t_out *out, t_data *data, t_ustr *buf);

// get_decimal.c
uint32_t	get_decimal(t_in *in, t_out *out, t_data *data, t_ustr *buf);

// get_hexa.c
uint32_t	get_hexa(t_in *in, t_out *out, t_data *data, t_ustr *buf);

// get_octal.c
uint32_t	get_octal(t_in *in, t_out *out, t_data *data, t_ustr *buf);

// get_directive.c
uint32_t	get_directive(t_in *in, t_out *out, t_data *data, t_ustr *buf);

// get_loop.c
uint32_t	get_loop(t_in *in, t_out *out, t_data *data, t_ustr *buf);

// get_number.c
uint8_t		get_number(t_in *in, uint32_t *n);

// get_string.c
uint32_t	get_string(t_in *in, t_out *out, t_data *data, t_ustr *buf);

// set_base.c
uint32_t	set_base(t_in *in, t_out *out, t_data *data, t_ustr *buf);

// skip_comment.c
uint32_t	skip_comment(t_in *in, t_out *out, t_data *data, t_ustr *buf);

#endif
