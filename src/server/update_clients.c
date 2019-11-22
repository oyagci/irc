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
#include <unistd.h>

void	remove_from_all_channels(struct s_server *self,
	struct s_client *const c)
{
	for (t_list *l = self->channels; l != NULL; l = l->next) {
		struct s_channel *chan = l->content;

		rm_from_chan(c->nickname, chan);
		notifypart(self, chan, c->nickname);
	}
}

int		update_clients(struct s_server *self)
{
	t_client	*c;
	size_t		i;

	i = 0;
	while (i < self->nclients)
	{
		c = self->clients + i;
		if (c->should_be_disconnected)
		{
			close(c->fd);
			remove_from_all_channels(self, c);
			nickremove(&c->server->nicks, c->nickname);
			del_client(self, c);
		}
		i += 1;
	}
	return (0);
}
