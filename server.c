/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oyagci <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/01 10:08:24 by oyagci            #+#    #+#             */
/*   Updated: 2019/08/09 16:38:22 by oyagci           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <bsd/string.h>

#include "server.h"
#include "logger.h"

/*
** Read data sent by on a file descriptor
** Reads at most 512 bytes
*/
int	read_client_command(int cfd, struct s_client_buffer *buffer)
{
	int	ret;

	if ((ret = read(cfd, buffer->data + buffer->len,
					COMMAND_LENGTH - buffer->len)) > 0)
	{
		buffer->len += ret;
		if (strstr(buffer->data, CRLF)) {
			buffer->is_complete = 1;
		}
	}
	return (ret);
}

int	irc_pass(struct s_client *c, char **params, int nparams)
{
	if (c->is_connected)
	{
		server_queue_code_reply(c->server, c, ERR_ALREADYREGISTRED);
		return (ERR_ALREADYREGISTRED);
	}
	if (SERVER_PASS)
	{
		if (nparams < 1)
		{
			server_queue_code_reply(c->server, c, ERR_NEEDMOREPARAM);
			return (ERR_NEEDMOREPARAM);
		}
		if (ft_strcmp(params[0], SERVER_PASS))
		{
			server_queue_code_reply(c->server, c, ERR_PASSWDMISMATCH);
			return (ERR_PASSWDMISMATCH);
		}
	}
	c->is_connected = 1;
	return (0);
}

int	irc_nick(struct s_client *c, char **params, int nparams)
{
	char	*nick;

	if (nparams < 1)
	{
		server_queue_code_reply(c->server, c, ERR_NONICKNAMEGIVEN);
		return (ERR_NONICKNAMEGIVEN);
	}
	nick = params[0];

	if (!nickavail(nick))
	{
		LOG(LOGDEBUG, "Nickname %.9s is already in use", nick);
		server_queue_code_reply(c->server, c, ERR_NICKNAMEINUSE);
		return (ERR_NICKNAMEINUSE);
	}
	if (ft_strlen(nick) > 9)
	{
		LOG(LOGDEBUG, "Nickname %.9s is too long (len > 9)", nick);
		server_queue_code_reply(c->server, c, ERR_ERRONEUSNICKNAME);
		return (ERR_ERRONEUSNICKNAME);
	}
	LOG(LOGDEBUG, "NICK %.9s -> %.9s", c->nickname, nick);
	strlcpy(c->nickname, nick, NICK_SIZE);
	nickadd(nick); /* TODO */
	return (0);
}

/*
**    Command: USER
** Parameters: <user> <mode> <unused> <realname>
*/
int	irc_user(struct s_client *c, char **params, int nparams)
{
	if (nparams < 4)
	{
		server_queue_code_reply(c->server, c, ERR_NEEDMOREPARAM);
		return (ERR_NEEDMOREPARAM);
	}
	if (c->is_registered)
		return (ERR_ALREADYREGISTRED);
	c->username = ft_strdup(params[0]);
	LOG(LOGDEBUG, "Username of %.9s set to %s", c->nickname, c->username);
	set_usermode(c, ft_atoi(params[1]));
	set_realname(c, params[3]);
	LOG(LOGDEBUG, "Realname set to %s", c->realname);
	server_queue_code_reply(c->server, c, RPL_WELCOME);
	return (0);
}

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

	elem = recipients;
	while (elem)
	{
		recipient = elem->content;
		formated = ft_strnew(512);
		ft_strlcat(formated, ":", 512);
		ft_strlcat(formated, from->nickname, 512);
		ft_strlcat(formated, " PRIVMSG ", 512);
		ft_strlcat(formated, recipient, 512);
		ft_strlcat(formated, " ", 512);
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
		server_queue_code_reply(client->server, client, ERR_NORECIPIENT);
		return (0);
	}
	if (nparams < 2)
	{
		server_queue_code_reply(client->server, client, ERR_NOTEXTTOSEND);
		return (0);
	}
	recipients = NULL;
	msgto(params[0], &recipients);
	if (recipients == NULL)
		server_queue_code_reply(client->server, client, ERR_NORECIPIENT);
	server_send_privmsg(client->server, client, recipients, params[1]);
	msgto_del(&recipients);
	return (0);
}

int	nparams(char **params)
{
	int ii;

	ii = 0;
	while (ii < 15 && params[ii] != 0)
		ii++;
	return (ii);
}

int	execute_command(struct s_client *c)
{
	const struct s_irc_cmds	cmds[] = {
		{ .name = "PASS", .f = irc_pass },
		{ .name = "NICK", .f = irc_nick },
		{ .name = "USER", .f = irc_user },
		{ .name = "JOIN", .f = irc_join },
		{ .name = "PRIVMSG", .f = irc_privmsg },
	};
	size_t					ii;
	struct s_message		*msg;
	int						validcmd;

	LOG(LOGDEBUG, "-> %s", c->buffer.data);
	msg = message(c->buffer.data);
	if (!msg)
		return (0);
	ii = 0;
	validcmd = 0;
	while (ii < sizeof(cmds) / sizeof(struct s_irc_cmds))
	{
		if (ft_strequ(cmds[ii].name, msg->cmd->data))
		{
			validcmd = 1;
			cmds[ii].f(c, msg->params->param, nparams(msg->params->param));
			break ;
		}
		ii++;
	}
	message_del(&msg);
	if (!validcmd)
		server_queue_code_reply(c->server, c, ERR_UNKNOWNCOMMAND);
	return (0);
}

int	main(int ac, char *av[])
{
	struct s_server		server;

	if (ac < 2) {
		printf("Usage: %s <port>\n", av[0]);
		exit(EXIT_FAILURE);
	}
	server_init(&server, ft_atoi(av[1]));
	server_loop(&server);
	return (0);
}
