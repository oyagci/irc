/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   update_clients.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oyagci <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/09/16 11:21:14 by oyagci            #+#    #+#             */
/*   Updated: 2019/09/16 11:21:28 by oyagci           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "server.h"

void	remove_from_all_channels(struct s_server *self,
	struct s_client *const c)
{
	t_list				*l;
	struct s_channel	*chan;

	l = self->channels;
	while (l != NULL)
	{
		chan = l->content;
		rm_from_chan(c->nickname, chan);
		self->notifypart(self, chan, c->nickname);
		l = l->next;
	}
}

int		update_clients(struct s_server *self)
{
	t_list			*elem;
	struct s_client	*c;

	elem = self->clients;
	while (elem)
	{
		c = elem->content;
		if (c->should_be_freed != 0)
		{
			remove_from_all_channels(self, c);
			nickremove(&c->server->nicks, c->nickname);
			self->del_client(self, c);
			elem = self->clients;
			if (!elem)
				break ;
		}
		elem = elem->next;
	}
	return (0);
}