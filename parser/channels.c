#include "libft.h"

int		chanstr(char const *inputc, char **buffer)
{
	unsigned char const	*input;
	int					ii;

	input = (unsigned char const *)inputc;
	if (*input == '#' || *input == '+' || *input == '&')
	{
		ii = 1;
		while (input[ii] != '\0' &&
				((input[ii] >= 0x01 && input[ii] <= 0x07) ||
				 (input[ii] >= 0x08 && input[ii] <= 0x09) ||
				 (input[ii] >= 0x0b && input[ii] <= 0x0c) ||
				 (input[ii] >= 0x0e && input[ii] <= 0x1f) ||
				 (input[ii] >= 0x2d && input[ii] <= 0x39) ||
				 (input[ii] >= 0x3b)))
		{
			ii++;
		}
		if (ii > 1 && ii <= 50)
		{
			*buffer = ft_strndup((char *)inputc, ii);
			return (ii);
		}
	}
	return (0);
}

/*
** channels: <chanstr> *( "," <chanstr> )
*/
t_list		*channels(char *input)
{
	t_list	*chans;
	t_list	*elem;
	int		ret;
	char	*chanbuf;

	chans = NULL;
	while ((ret = chanstr(input, &chanbuf)) > 0)
	{
		elem = ft_lstnew(0, 0);
		elem->content = chanbuf;
		ft_lstpush(&chans, elem);
		input += ret;
		if (*input == ',')
			input++;
	}
	return (chans);
}
