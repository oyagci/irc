#include "channels.h"

struct s_chan	*channels_get(struct s_channels *const self, char const *const name)
{
	t_list			*elem;
	struct s_chan	*c;

	elem = self->list;
	while (elem)
	{
		c = elem->content;
		if (ft_strequ(c->name, name))
			return (c);
		elem = elem->next;
	}
	return (NULL);
}

void	channels_init(struct s_channels *ptr)
{
	ptr->get = &channels_get;
}
