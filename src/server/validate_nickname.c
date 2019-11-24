#include "parser.h"

static int	is_valid_char(char c)
{
	return (letter(c) || special(c) || digit(c) || c == '-');
}

int			validate_nickname(char const *input)
{
	int	ii;

	ii = 0;
	if (letter(*input) || special(*input))
	{
		ii += 1;
		while (input[ii] && is_valid_char(input[ii]))
			ii += 1;
		if (input[ii] != ' ' && input[ii] != '\0')
			return (0);
	}
	if (ii < 9 && ii > 0)
		return (ii);
	return (0);
}
