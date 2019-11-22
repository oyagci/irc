/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server_channel.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oyagci <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/09/09 14:27:37 by oyagci            #+#    #+#             */
/*   Updated: 2019/11/22 13:35:07 by oyagci           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>

#include "server.h"
#include "libft.h"

struct s_channel *create_channel(struct s_server *server, char const *name, int mode)
{
	t_list				*elem;
	struct s_channel	*chan;

	elem = server->channels;
	while (elem)
	{
		chan = elem->content;
		if (ft_strequ(chan->name, name))
			return (NULL);
		elem = elem->next;
	}
	chan = ft_memalloc(sizeof(*chan));
	if (!chan)
	{
		exit(EXIT_FAILURE);
		return (NULL);
	}
	ft_strcpy(chan->name, name);
	chan->mode = mode;
	elem = ft_lstnew(0, 0);
	elem->content = chan;
	ft_lstpush(&server->channels, elem);
	return (chan);
}

struct s_channel	*get_channel(struct s_server *server, char const *name)
{
	struct s_channel *chan;

	chan = NULL;
	for (t_list *elem = server->channels; elem != NULL; elem = elem->next) {
		struct s_channel *cur = elem->content;

		if (ft_strequ(cur->name, name)) {
			chan = cur;
			break ;
		}
	}
	return (chan);
}

int add_to_chan(t_server *server, t_client *client, char const *const channame)
{
	struct s_channel *chan = NULL;

	for (t_list *elem = server->channels; elem != NULL; elem = elem->next)
	{
		struct s_channel *cur = elem->content;

		if (ft_strequ(channame, cur->name))
		{
			chan = cur;
			break ;
		}
	}

	if (!chan)
		chan = create_channel(server, channame, 0);
	if (!channel_add_client(chan, client))
		server_tell_new_client(server, client, chan);
	return (0);
}

int rm_from_chan(char *nick, t_channel *chan)
{
	t_list			*clients;
	t_list			*prev;
	struct s_client	*c;

	clients = chan->clients;
	prev = NULL;
	while (clients) {
		c = clients->content;
		if (ft_strequ(c->nickname, nick)) {
			if (prev) {
				prev->next = clients->next;
			}
			else {
				chan->clients = clients->next;
			}
			free(clients);
			return (0);
		}
		prev = clients;
		clients = clients->next;
	}
	return (ERR_NOTONCHANNEL);
}
