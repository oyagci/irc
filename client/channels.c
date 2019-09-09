/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   channels.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oyagci <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/09/09 13:02:47 by oyagci            #+#    #+#             */
/*   Updated: 2019/09/09 13:04:52 by oyagci           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "channels.h"
#include <stdlib.h>

struct s_chan	*channels_get(struct s_channels *const self,
					char const *const name)
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

int				channels_add(struct s_channels *self, struct s_chan *chan)
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

int				channels_addnick(struct s_channels *self,
					char const *const nick,
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
	if (!(c = ft_memalloc(sizeof(*c))))
		exit(EXIT_FAILURE);
	ft_strncpy(c->name, name, 50);
	return (c);
}

void			channels_init(struct s_channels *ptr)
{
	ptr->list = 0;
	ptr->get = &channels_get;
	ptr->addnick = &channels_addnick;
	ptr->create = &channels_create;
	ptr->add = &channels_add;
	ptr->rmnick = &channels_rmnick;
}
