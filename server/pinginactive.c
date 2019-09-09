/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pinginactive.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oyagci <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/09/09 14:27:36 by oyagci            #+#    #+#             */
/*   Updated: 2019/09/09 14:27:40 by oyagci           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "server.h"
#include <time.h>

int		updatetimeout(struct s_server *self)
{
	t_list			*clients;
	struct s_client	*client;
	struct timespec	current;

	clock_gettime(CLOCK_REALTIME, &current);
	clients = self->clients;
	while (clients)
	{
		client = clients->content;
		if (client->timeout.tv_sec != 0 &&
			current.tv_sec >= client->timeout.tv_sec)
		{
			self->quit(self, client, "Ping timeout exceeded");
			ft_memset(&client->timeout, 0, sizeof(client->timeout));
		}
		clients = clients->next;
	}
	return (0);
}

void	set_timeout(struct s_client *c, unsigned sec)
{
	struct timespec	t;

	clock_gettime(CLOCK_REALTIME, &t);
	c->timeout.tv_sec = t.tv_sec + sec;
	c->timeout.tv_nsec = 0;
}

int		pinginactive(struct s_server *self)
{
	t_list			*clients;
	struct s_client	*client;
	struct timespec	current;

	clients = self->clients;
	updatetimeout(self);
	while (clients)
	{
		client = clients->content;
		clock_gettime(CLOCK_REALTIME, &current);
		if (client->timeout.tv_sec == 0 &&
			current.tv_sec >= client->ping.tv_sec)
		{
			irc_ping(client, 0, 0);
			clock_gettime(CLOCK_REALTIME, &client->ping);
			client->ping.tv_sec += 20;
			set_timeout(client, 10);
		}
		clients = clients->next;
	}
	return (0);
}
