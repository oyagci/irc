/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   channel.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oyagci <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/09/09 14:27:36 by oyagci            #+#    #+#             */
/*   Updated: 2019/09/09 14:27:40 by oyagci           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "server.h"
#include "logger.h"
#include <stdlib.h>

int		client_in_channel(struct s_client const *const client,
		struct s_channel const *const channel)
{
	t_list			*elem;
	struct s_client	*c;
	int				ret;

	ret = 0;
	elem = channel->clients;
	while (elem)
	{
		c = elem->content;
		if (ft_strnequ(client->nickname, c->nickname, NICK_SIZE))
		{
			ret = -1;
			break ;
		}
		elem = elem->next;
	}
	return (ret);
}

int		channel_add_client(struct s_channel *channel, struct s_client *client)
{
	t_list	*elem;
	int		ret;

	ret = 0;
	elem = ft_lstnew(0, 0);
	if (!elem)
		return (-1);
	elem->content = client;
	if (!client_in_channel(client, channel))
	{
		ft_lstpush(&channel->clients, elem);
		LOG(LOGDEBUG, "Client %s added to channel %.50s", client->nickname,
			channel->name);
	}
	else
	{
		free(elem);
		ret = -1;
	}
	return (ret);
}

int		channel_rm_nick(struct s_channel *const channel, char const *const nick)
{
	t_list			*prev;
	t_list			*clients;
	struct s_client	*client;

	clients = channel->clients;
	while (clients)
	{
		client = clients->content;
		if (ft_strnequ(client->nickname, nick, NICK_SIZE))
		{
			if (prev)
				prev->next = clients->next;
			else
				channel->clients = clients->next;
		}
		prev = clients;
		clients = clients->next;
	}
	return (0);
}
