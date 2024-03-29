/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   irc_privmsg.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oyagci <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/09/09 14:27:36 by oyagci            #+#    #+#             */
/*   Updated: 2019/11/22 13:17:58 by oyagci           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "server.h"

struct s_client	*server_get_client(struct s_server *server, char const *nick)
{
	struct s_client	*client;
	size_t			i;

	i = 0;
	while (i < NCLIENTS)
	{
		client = server->clients + i;
		if (client->fd > 0 && ft_strequ(client->nickname, nick))
			return (client);
		i += 1;
	}
	return (NULL);
}

int				server_send_channel(struct s_server *server,
	struct s_channel *chan, char *msg)
{
	t_list		*client;
	t_client	*recipient;

	client = chan->clients;
	while (client)
	{
		recipient = client->content;
		queue_reply(server, recipient, msg);
		client = client->next;
	}
	return (0);
}

int				server_send_formated_message_to(struct s_server *server,
	char const *name, char *msg)
{
	struct s_channel	*chan;
	struct s_client		*client;

	if (*name == '#' || *name == '&' || *name == '!')
	{
		chan = get_channel(server, name);
		if (chan)
			server_send_channel(server, chan, msg);
		else
		{
			return (ERR_CANNOTSENDTOCHAN);
		}
	}
	else
	{
		if ((client = server_get_client(server, name)))
			queue_reply(server, client, msg);
		return (ERR_NOSUCHNICK);
	}
	return (0);
}

int				server_send_privmsg(struct s_server *server,
		struct s_client *from, t_list *recipients, char const *msg)
{
	t_list	*elem;
	char	*formated;
	char	*recipient;

	if (!msg)
		return (1);
	elem = recipients;
	while (elem)
	{
		recipient = elem->content;
		formated = ft_strnew(512);
		ft_strlcat(formated, ":", 512);
		ft_strlcat(formated, from->nickname, 512);
		ft_strlcat(formated, " PRIVMSG ", 512);
		ft_strlcat(formated, recipient, 512);
		ft_strlcat(formated, " :", 512);
		ft_strlcat(formated, msg, 512);
		ft_strlcat(formated, CRLF, 512);
		server_send_formated_message_to(server, recipient, formated);
		free(formated);
		elem = elem->next;
	}
	return (0);
}

int				irc_privmsg(struct s_client *client, struct s_params *p)
{
	t_list	*recipients;

	if (!client->is_registered)
		return (0);
	if (p->nparam < 1)
	{
		queue_code_reply(client->server, client, ERR_NORECIPIENT);
		return (0);
	}
	if (p->nparam < 2)
	{
		queue_code_reply(client->server, client, ERR_NOTEXTTOSEND);
		return (0);
	}
	recipients = NULL;
	msgto(p->param[0], &recipients);
	if (recipients == NULL)
	{
		queue_code_reply(client->server, client, ERR_NORECIPIENT);
		return (1);
	}
	server_send_privmsg(client->server, client, recipients, p->param[1]);
	msgto_del(&recipients);
	return (0);
}
