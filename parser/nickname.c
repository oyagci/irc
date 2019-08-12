#include "../server.h"

int			nickname(char const *input, char **buf)
{
	int	ii;

	ii = 0;
	if (letter(*input) || special(*input))
	{
		ii += 1;
		while (input[ii] &&
			(letter(input[ii]) || special(input[ii]) || digit(input[ii]) || input[ii] == '-'))
			ii += 1;
	}
	if (ii < 9 && ii > 0)
	{
		*buf = ft_strndup(input, ii);
		return (ii);
	}
	return (0);
}
