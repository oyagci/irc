/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   irc_join.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oyagci <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/09/09 14:27:36 by oyagci            #+#    #+#             */
/*   Updated: 2019/09/09 14:27:40 by oyagci           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "server.h"
#include <stdlib.h>

int	server_tell_new_client(struct s_server *server, struct s_client *client,
	struct s_channel *chan)
{
	t_list			*elem;
	char			*msg;
	struct s_client	*recipient;

	elem = chan->clients;
	while (elem)
	{
		recipient = elem->content;
		if (!(msg = ft_memalloc(sizeof(char) * 513)))
			exit(EXIT_FAILURE);
		ft_strlcat(msg, ":", 513);
		ft_strlcat(msg, client->nickname, 513);
		if (client->username)
		{
			ft_strlcat(msg, "!", 513);
			ft_strlcat(msg, client->username, 513);
		}
		ft_strlcat(msg, "@irc.42.fr JOIN ", 513);
		ft_strlcat(msg, chan->name, 513);
		ft_strlcat(msg, CRLF, 513);
		queue_reply(server, recipient, msg);
		free(msg);
		elem = elem->next;
	}
	return (0);
}

int	irc_join(struct s_client *client, char **params, int nparams)
{
	t_list	*l;
	t_list	*chans;
	char	*chan;

	if (!client->is_registered)
		return (0);
	chan = NULL;
	if (nparams < 1)
		return (ERR_NEEDMOREPARAM);
	chans = channels(params[0]);
	l = chans;
	while (l)
	{
		chan = l->content;
		client->server->add_to_chan(client->server, client, chan);
		l = l->next;
	}
	channels_del(&chans);
	return (0);
}
