#include "libft.h"
#include "channels.h"
#include <stdlib.h>

static void		rmnick_iter(t_list **list, char const *const nick)
{
	t_list	*next;
	t_list	*prev;
	t_list	*elem;
	char	*n;

	next = 0;
	prev = 0;
	elem = *list;
	while (elem)
	{
		next = elem->next;
		n = elem->content;
		if (ft_strequ(nick, n))
		{
			free(n);
			if (prev)
				prev->next = elem->next;
			else
				*list = elem->next;
			free(elem);
		}
		prev = elem;
		elem = next;
	}
}

int				channels_rmnick(struct s_channels *self, char const *const nick,
						char const *const channel)
{
	struct s_chan	*c;

	if (!(c = self->get(self, channel)))
		return (0);
	rmnick_iter(&c->clients, nick);
	return (0);
}
