#include "channels.h"
#include <stdlib.h>

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

int		channels_add(struct s_channels *self, struct s_chan *chan)
{
	t_list	*elem;

	elem = ft_lstnew(0, 0);
	if (!elem)
		return (-1);
	elem->content = chan;
	elem->content_size = sizeof(*chan);
	ft_lstpush(&self->list, elem);
	return (0);
}

int		channels_addnick(struct s_channels *self, char const *const nick,
						   char const *const chan)
{
	struct s_chan	*c;
	t_list			*elem;

	c = self->get(self, chan);
	if (!c)
	{
		LOG(LOGDEBUG, "Creating channel %s", chan);
		c = self->create(chan);
		self->add(self, c);
	}
	elem = ft_lstnew(0, 0);
	elem->content = ft_strdup(nick);
	ft_lstpush(&c->clients, elem);
	return (0);
}

struct s_chan	*channels_create(char const *const name)
{
	struct s_chan	*c;

	if (ft_strlen(name) > 50)
		return (NULL);
	c = ft_memalloc(sizeof(*c));
	ft_strncpy(c->name, name, 50);
	return (c);
}

int		channels_rmnick(struct s_channels *self, char const *const nick,
						char const *const channel)
{
	struct s_chan	*c;
	t_list			*elem;
	char			*n;
	t_list			*prev;
	t_list			*next;

	c = self->get(self, channel);
	if (c)
	{
		prev = 0;
		elem = c->clients;
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
					c->clients = elem->next;
				free(elem);
			}
			prev = elem;
			elem = next;
		}
	}
	return (0);
}

void	channels_init(struct s_channels *ptr)
{
	ptr->list = 0;
	ptr->get = &channels_get;
	ptr->addnick = &channels_addnick;
	ptr->create = &channels_create;
	ptr->add = &channels_add;
	ptr->rmnick = &channels_rmnick;
}
