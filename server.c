/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oyagci <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/01 10:08:24 by oyagci            #+#    #+#             */
/*   Updated: 2019/08/06 16:18:19 by oyagci           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <netdb.h>
#include <stdio.h>

#include "irc.h"
#include "logger.h"

int		channel(char const *inputc, char **buffer)
{
	unsigned char const	*input;
	int					ii;

	input = (unsigned char const *)inputc;
	if (*input == '#' || *input == '+' || *input == '&')
	{
		ii = 1;
		while (input[ii] != '\0' &&
				((input[ii] >= 0x01 && input[ii] <= 0x07) ||
				 (input[ii] >= 0x08 && input[ii] <= 0x09) ||
				 (input[ii] >= 0x0b && input[ii] <= 0x0c) ||
				 (input[ii] >= 0x0e && input[ii] <= 0x1f) ||
				 (input[ii] >= 0x2d && input[ii] <= 0x39) ||
				 (input[ii] >= 0x3b)))
		{
			ii++;
		}
		if (ii > 1)
		{
			*buffer = ft_strndup((char *)inputc, ii);
			return (ii);
		}
	}
	return (0);
}

t_list		*parse_param_channels(char *input)
{
	t_list	*chans;
	t_list	*elem;
	int		ret;
	char	*chanbuf;

	chans = NULL;
	while ((ret = channel(input, &chanbuf)) > 0)
	{
		input += ret;
		elem = ft_lstnew(0, 0);
		elem->content = chanbuf;
		ft_lstpush(&chans, elem);
	}
	return (chans);
}

/* TODO */
int		tell_client_topic(struct s_client *c, struct s_channel *chan)
{
	send(c->fd, chan->topic, ft_strlen(chan->topic), 0);
	return (0);
}

int	read_client_command(int cfd, struct s_client_buffer *buffer)
{
	int	ret;

	/*
	** RFC2812 limits the maximum length of a command sent by
	** the client to 512 bytes at most.
	*/
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
	LOG(LOGDEBUG, "PASS from %d", c->fd);
	if (c->is_connected)
		return (ERR_ALREADYREGISTRED);
	if (SERVER_PASS)
	{
		if (nparams < 1)
			return (ERR_NEEDMOREPARAM);
		if (ft_strcmp(params[0], SERVER_PASS))
			return (ERR_PASSWDMISMATCH);
	}
	c->is_connected = 1;
	return (0);
}

int	irc_nick(struct s_client *c, char **params, int nparams)
{
	char	*nick;

	LOG(LOGDEBUG, "[%d;%.9s] NICK", c->fd, c->nickname);

	if (nparams < 1)
		return (ERR_NONICKNAMEGIVEN);
	nick = params[0];

	LOG(LOGDEBUG, "Old nick: %.9s", c->nickname);
	LOG(LOGDEBUG, "New nick: %.9s", nick);

	if (!nickavail(nick))
	{
		LOG(LOGDEBUG, "Nickname %.9s is already in use", nick);
		return (ERR_NICKNAMEINUSE);
	}
	if (ft_strlen(nick) > 9)
	{
		LOG(LOGDEBUG, "Nickname %.9s is too long (len > 9)", nick);
		return (ERR_ERRONEUSNICKNAME);
	}
	strlcpy(c->nickname, nick, NICK_SIZE);
	nickadd(nick);
	LOG(LOGDEBUG, "Nickname set to %.9s", c->nickname);
	return (0);
}

/*
**    Command: USER
** Parameters: <user> <mode> <unused> <realname>
*/
int	irc_user(struct s_client *c, char **params, int nparams)
{
	if (nparams < 4) {
		LOG(LOGDEBUG, "[%d;%.9s] Not enough parameters", c->fd, c->nickname);
		return (ERR_NEEDMOREPARAM);
	}
	if (c->is_registered) {
		return (ERR_ALREADYREGISTRED);
	}
	c->username = ft_strdup(params[0]);
	LOG(LOGDEBUG, "Username of %.9s set to %s\n", c->nickname, c->username);
	set_usermode(c, ft_atoi(params[1]));
	set_realname(c, params[3]);
	LOG(LOGDEBUG, "Realname set to %s", c->realname);
	return (1);
}

/* TODO */
int	irc_join(struct s_client *c, char **params, int nparams)
{
	t_list				*chans;
	struct s_channel	*chan;

	chan = NULL;
	if (nparams < 1)
		return (ERR_NEEDMOREPARAM);

	chans = parse_param_channels(params[0]);
	tell_client_topic(c, chan);
	return (0);
}

int	irc_privmsg(struct s_client *c, char **params, int nparams)
{
	(void)c;
	(void)params;
	(void)nparams;
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
	int						ret;

	msg = message(c->buffer.data);
	ii = 0;
	while (ii < sizeof(cmds) / sizeof(struct s_irc_cmds))
	{
		if (ft_strequ(cmds[ii].name, msg->cmd->data))
			ret = cmds[ii].f(c, msg->params->param,
					nparams(msg->params->param));
		ii++;
	}
	return (ret);
}

int	main(int ac, char *av[])
{
	struct sockaddr_in	serv_addr;
	int					max_sd;
	struct s_server		server;

	server.clients = NULL;
	ft_memset(&server, 0, sizeof(server));
	if (ac < 2) {
		printf("Usage: %s <port>\n", av[0]);
		exit(EXIT_FAILURE);
	}

	LOG(LOGDEBUG, "Creating server on port %s", av[1]);

	server.sockfd = socket(AF_INET, SOCK_STREAM, 0);

	memset(&serv_addr, 0, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = INADDR_ANY;
	serv_addr.sin_port = htons(atoi(av[1]));

	bind(server.sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr));
	listen(server.sockfd, MAX_CONN);

	while (42) {
		max_sd = set_fds(server.sockfd, server.clients, &server.readfds, &server.writefds);
		select(max_sd + 1, &server.readfds, &server.writefds, NULL, NULL);
		accept_new_clients(&server);
		handle_io_clients(&server, server.clients);
	}
	return (0);
}
