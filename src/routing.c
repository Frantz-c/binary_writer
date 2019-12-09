#include "interpret_file.h"

uint8_t	get_func_index[TOTAL_BASES][0x100] =
{
	{	// ALL_BASES
		-1,-1,-1,-1,-1,-1,-1,-1,
		-1,-1,-1,-1,-1,-1,-1,-1,
		-1,-1,-1,-1,-1,-1,-1,-1,
		-1,-1,-1,-1,-1,-1,-1,-1,
		-1,-1,I_STRING, I_COMMENT,	// ' ' '!' '"' '#'
		I_B2,I_DIRECTIVE,-1,I_CHAR,	// '$' '%' '&' '\''
		I_LOOP,-1,-1,I_B10,			// '(' ')' '*' '+'
		-1,-1,-1,-1,				// ',' '-' '.' '/'
		I_B8,I_B10,I_B10,I_B10,		// '0' '1' '2' '3'
		I_B10,I_B10,I_B10,I_B10,	// '4' '5' '6' '7'
		I_B10,I_B10,-1,I_COMMENT,	// '8' '9' ':' ';'
		I_BASE,-1,-1,-1,			// '<' '=' '>' '?'
		-1,-1,-1,-1,				// '@' 'A' 'B' 'C'
		-1,-1,-1,-1,				// 'D' 'E' 'F' 'G'
		-1,-1,-1,-1,				// 'H' 'I' 'J' 'K'
		-1,-1,-1,-1,				// 'L' 'M' 'N' 'O'
		-1,-1,-1,-1,				// 'P' 'Q' 'R' 'S'
		-1,-1,-1,-1,				// 'T' 'U' 'V' 'W'
		-1,-1,-1,I_AUTOPAD,			// 'X' 'Y' 'Z' '['
		-1,-1,-1,-1,				// '\\' ']' '^' '_'
		-1,-1,-1,-1,				// '`' 'a' 'b' 'c'
		-1,-1,-1,-1,				// 'd' 'e' 'f' 'g'
		I_B16,-1,-1,-1,				// 'h' 'i' 'j' 'k'
		-1,-1,-1,I_B8,				// 'l' 'm' 'n' 'o'
		-1,-1,-1,-1,				// 'p' 'q' 'r' 's'
		-1,-1,-1,-1,				// 't' 'u' 'v' 'w'
		I_B16,-1,-1,-1,				// 'x' 'y' 'z' '{'
		-1,-1,-1,-1					// '|' '}' '~' DEL
	},
	{	// HEXA_BASE
		-1,-1,-1,-1,-1,-1,-1,-1,
		-1,-1,-1,-1,-1,-1,-1,-1,
		-1,-1,-1,-1,-1,-1,-1,-1,
		-1,-1,-1,-1,-1,-1,-1,-1,
		-1,-1,I_STRING, I_COMMENT,	// ' ' '!' '"' '#'
		I_B2,I_DIRECTIVE,-1,I_CHAR,	// '$' '%' '&' '\''
		I_LOOP,-1,-1,I_B10,			// '(' ')' '*' '+'
		-1,-1,-1,-1,				// ',' '-' '.' '/'
		I_B16,I_B16,I_B16,I_B16,	// '0' '1' '2' '3'
		I_B16,I_B16,I_B16,I_B16,	// '4' '5' '6' '7'
		I_B16,I_B16,-1,I_COMMENT,	// '8' '9' ':' ';'
		I_BASE,-1,-1,-1,			// '<' '=' '>' '?'
		-1,I_B16,I_B16,I_B16,		// '@' 'A' 'B' 'C'
		I_B16,I_B16,I_B16,-1,		// 'D' 'E' 'F' 'G'
		-1,-1,-1,-1,				// 'H' 'I' 'J' 'K'
		-1,-1,-1,-1,				// 'L' 'M' 'N' 'O'
		-1,-1,-1,-1,				// 'P' 'Q' 'R' 'S'
		-1,-1,-1,-1,				// 'T' 'U' 'V' 'W'
		-1,-1,-1,I_AUTOPAD,			// 'X' 'Y' 'Z' '['
		-1,-1,-1,-1,				// '\\' ']' '^' '_'
		-1,I_B16,I_B16,I_B16,		// '`' 'a' 'b' 'c'
		I_B16,I_B16,I_B16,-1,		// 'd' 'e' 'f' 'g'
		I_B16,-1,-1,-1,				// 'h' 'i' 'j' 'k'
		-1,-1,-1,I_B8,				// 'l' 'm' 'n' 'o'
		-1,-1,-1,-1,				// 'p' 'q' 'r' 's'
		-1,-1,-1,-1,				// 't' 'u' 'v' 'w'
		I_B16,-1,-1,-1,				// 'x' 'y' 'z' '{'
		-1,-1,-1,-1					// '|' '}' '~' DEL
	},
	{	// OCTAL_BASE
		-1,-1,-1,-1,-1,-1,-1,-1,
		-1,-1,-1,-1,-1,-1,-1,-1,
		-1,-1,-1,-1,-1,-1,-1,-1,
		-1,-1,-1,-1,-1,-1,-1,-1,
		-1,-1,I_STRING, I_COMMENT,	// ' ' '!' '"' '#'
		I_B2,I_DIRECTIVE,-1,I_CHAR,	// '$' '%' '&' '\''
		I_LOOP,-1,-1,I_B10,			// '(' ')' '*' '+'
		-1,-1,-1,-1,				// ',' '-' '.' '/'
		I_B8,I_B8,I_B8,I_B8,		// '0' '1' '2' '3'
		I_B8,I_B8,I_B8,I_B8,		// '4' '5' '6' '7'
		-1,-1,-1,I_COMMENT,			// '8' '9' ':' ';'
		I_BASE,-1,-1,-1,			// '<' '=' '>' '?'
		-1,-1,-1,-1,				// '@' 'A' 'B' 'C'
		-1,-1,-1,-1,				// 'D' 'E' 'F' 'G'
		-1,-1,-1,-1,				// 'H' 'I' 'J' 'K'
		-1,-1,-1,-1,				// 'L' 'M' 'N' 'O'
		-1,-1,-1,-1,				// 'P' 'Q' 'R' 'S'
		-1,-1,-1,-1,				// 'T' 'U' 'V' 'W'
		-1,-1,-1,I_AUTOPAD,			// 'X' 'Y' 'Z' '['
		-1,-1,-1,-1,				// '\\' ']' '^' '_'
		-1,-1,-1,-1,				// '`' 'a' 'b' 'c'
		-1,-1,-1,-1,				// 'd' 'e' 'f' 'g'
		I_B16,-1,-1,-1,				// 'h' 'i' 'j' 'k'
		-1,-1,-1,I_B8,				// 'l' 'm' 'n' 'o'
		-1,-1,-1,-1,				// 'p' 'q' 'r' 's'
		-1,-1,-1,-1,				// 't' 'u' 'v' 'w'
		I_B16,-1,-1,-1,				// 'x' 'y' 'z' '{'
		-1,-1,-1,-1					// '|' '}' '~' DEL
	},
	{	// BINARY_BASE
		-1,-1,-1,-1,-1,-1,-1,-1,
		-1,-1,-1,-1,-1,-1,-1,-1,
		-1,-1,-1,-1,-1,-1,-1,-1,
		-1,-1,-1,-1,-1,-1,-1,-1,
		-1,-1,I_STRING, I_COMMENT,	// ' ' '!' '"' '#'
		I_B2,I_DIRECTIVE,-1,I_CHAR,	// '$' '%' '&' '\''
		I_LOOP,-1,-1,I_B10,			// '(' ')' '*' '+'
		-1,-1,-1,-1,				// ',' '-' '.' '/'
		I_B2,I_B2,-1,-1,			// '0' '1' '2' '3'
		-1,-1,-1,-1,				// '4' '5' '6' '7'
		-1,-1,-1,I_COMMENT,			// '8' '9' ':' ';'
		I_BASE,-1,-1,-1,			// '<' '=' '>' '?'
		-1,-1,-1,-1,				// '@' 'A' 'B' 'C'
		-1,-1,-1,-1,				// 'D' 'E' 'F' 'G'
		-1,-1,-1,-1,				// 'H' 'I' 'J' 'K'
		-1,-1,-1,-1,				// 'L' 'M' 'N' 'O'
		-1,-1,-1,-1,				// 'P' 'Q' 'R' 'S'
		-1,-1,-1,-1,				// 'T' 'U' 'V' 'W'
		-1,-1,-1,I_AUTOPAD,			// 'X' 'Y' 'Z' '['
		-1,-1,-1,-1,				// '\\' ']' '^' '_'
		-1,-1,-1,-1,				// '`' 'a' 'b' 'c'
		-1,-1,-1,-1,				// 'd' 'e' 'f' 'g'
		I_B16,-1,-1,-1,				// 'h' 'i' 'j' 'k'
		-1,-1,-1,I_B8,				// 'l' 'm' 'n' 'o'
		-1,-1,-1,-1,				// 'p' 'q' 'r' 's'
		-1,-1,-1,-1,				// 't' 'u' 'v' 'w'
		I_B16,-1,-1,-1,				// 'x' 'y' 'z' '{'
		-1,-1,-1,-1					// '|' '}' '~' DEL
	}
};

uint32_t	(*call[I_END])(t_in*,t_out*,t_data*,t_ustr*) =
{
	&get_string,	&skip_comment,	&get_directive,
	&get_character,	&get_loop,		&get_octal,
	&get_decimal,	&get_hexa,		&get_binary,
	&set_base,		&get_autopad
};