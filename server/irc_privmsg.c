#include <stdlib.h>

#include "server.h"
#include "logger.h"

struct s_client	*server_get_client(struct s_server *server, char const *nick)
{
	t_list			*clients;
	struct s_client	*client;

	clients = server->clients;
	while (clients)
	{
		client = clients->content;
		if (ft_strequ(client->nickname, nick))
			return (client);
		clients = clients->next;
	}
	return (NULL);
}

/*
** Send a message to every member of a channel
*/
int	server_send_channel(struct s_server *server, struct s_channel *chan,
	char *msg)
{
	t_list			*client;
	struct s_client	*recipient;

	client = chan->clients;
	while (client)
	{
		recipient = client->content;
		server_queue_reply(server, recipient, msg);
		client = client->next;
	}
	return (0);
}

/*
** Send a message to a channel or a nickname
*/
int	server_send_formated_message_to(struct s_server *server, char const *name,
	char *msg)
{
	struct s_channel	*chan;
	struct s_client		*client;

	if (*name == '#' || *name == '&' || *name == '!')
	{
		chan = server_get_channel(server, name);
		if (chan)
			server_send_channel(server, chan, msg);
		else
		{
			LOG(LOGDEBUG, "No channel named %s", name);
			/* TODO: Send ERR_CANNOTSENDTOCHAN */
		}
	}
	else
	{
		client = server_get_client(server, name);
		server_queue_reply(server, client, msg);
	}
	return (0);
}

int	server_send_privmsg(struct s_server *server, struct s_client *from,
		t_list *recipients, char const *msg)
{
	t_list	*elem;
	char	*formated;
	char	*recipient;

	if (!msg)
	{
		LOG(LOGWARN, "%s", __FUNCTION__);
		return (1);
	}
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
		LOG(LOGDEBUG, "To %s: %s", recipient, msg);
		server_send_formated_message_to(server, recipient, formated);
		free(formated);
		elem = elem->next;
	}
	return (0);
}

int	irc_privmsg(struct s_client *client, char **params, int nparams)
{
	t_list	*recipients;

	if (nparams < 1)
	{
		client->server->queuecode(client->server, client, ERR_NORECIPIENT);
		return (0);
	}
	if (nparams < 2)
	{
		client->server->queuecode(client->server, client, ERR_NOTEXTTOSEND);
		return (0);
	}
	recipients = NULL;
	msgto(params[0], &recipients);
	if (recipients == NULL)
	{
		client->server->queuecode(client->server, client, ERR_NORECIPIENT);
		return (1);
	}
	server_send_privmsg(client->server, client, recipients, params[1]);
	msgto_del(&recipients);
	return (0);
}
