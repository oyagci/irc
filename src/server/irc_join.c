/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   irc_join.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oyagci <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/09/09 14:27:36 by oyagci            #+#    #+#             */
/*   Updated: 2019/11/22 13:49:54 by oyagci           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "server.h"
#include <stdlib.h>

int	server_tell_new_client(t_server *server, t_client *client, t_channel *chan)
{
	t_list		*elem;
	char		*msg;
	t_client	*recipient;

	elem = chan->clients;
	while (elem)
	{
		recipient = elem->content;
		if (!(msg = ft_memalloc(sizeof(char) * 513)))
			exit(EXIT_FAILURE);
		ft_strlcat(msg, ":", 513);
		ft_strlcat(msg, client->nickname, 513);
		ft_strlcat(msg, "@irc.42.fr JOIN ", 513);
		ft_strlcat(msg, chan->name, 513);
		ft_strlcat(msg, CRLF, 513);
		queue_reply(server, recipient, msg);
		free(msg);
		elem = elem->next;
	}
	return (0);
}

int	irc_join(t_client *client, struct s_params *p)
{
	t_list	*chans;
	t_list	*l;
	char	*chan;

	chans = NULL;
	if (!client->is_registered)
		return (0);
	if (p->nparam < 1)
		return (ERR_NEEDMOREPARAM);
	chans = channels(p->param[0]);
	l = chans;
	while (l)
	{
		chan = l->content;
		add_to_chan(client->server, client, chan);
		l = l->next;
	}
	channels_del(&chans);
	return (0);
}
