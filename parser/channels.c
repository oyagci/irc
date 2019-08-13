#include "../libft/includes/libft.h"
#include "server.h"
#include <stdlib.h>

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

void		channels_delone(void *c, size_t s)
{
	(void)s;
	free(c);
}

void		channels_del(t_list **chansp)
{
	ft_lstdel(chansp, &channels_delone);
}
