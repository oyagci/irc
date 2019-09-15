/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   irc_who.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oyagci <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/09/09 14:27:36 by oyagci            #+#    #+#             */
/*   Updated: 2019/09/09 14:27:40 by oyagci           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "server.h"

int	irc_who_client(struct s_server *server, struct s_client *client,
	struct s_client *dest)
{
	char	msg[512];

	ft_memset(msg, 0, 512);
	ft_strlcat(msg, SRPL_WHOREPLY, 512);
	ft_strlcat(msg, " ", 512);
	ft_strlcat(msg, client->username, 512);
	ft_strlcat(msg, " ", 512);
	ft_strlcat(msg, "todo.42.fr", 512);
	ft_strlcat(msg, " ", 512);
	ft_strlcat(msg, "irc.42.fr", 512);
	ft_strlcat(msg, " ", 512);
	ft_strlcat(msg, client->nickname, 512);
	ft_strlcat(msg, "\n", 512);
	server->queuenotif(server, dest, msg);
	return (0);
}

#include <stdio.h>
int	irc_who(struct s_client *client, struct s_params *p)
{
	t_list			*clients;
	struct s_client	*c;
	struct s_channel const *chan;

	chan = client->server->get_channel(client->server, p->param[0]);
	if (chan)
	{
		clients = chan->clients;
		while (clients)
		{
			c = clients->content;
			irc_who_client(client->server, c, client);
			clients = clients->next;
		}
	}
	return (0);
}
