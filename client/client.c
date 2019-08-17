/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oyagci <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/01 10:08:25 by oyagci            #+#    #+#             */
/*   Updated: 2019/08/08 15:56:24 by oyagci           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "client.h"
#include "logger.h"

#include <stdio.h>
#include <stdlib.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <sys/time.h>

#include <netdb.h>
#include <string.h>
#include <unistd.h>
#include <stdio.h>
#include <errno.h>

int	eventmotd(struct s_client *const self, struct s_message const *const cmd)
{
	int	ii;
	(void)self;

	LOG(LOGDEBUG, "MOTD");
	ii = 0;
	while (cmd->params->param[ii] && ii < 14)
	{
		printf("%s\n", cmd->params->param[ii]);
		ii++;
	}
	return (0);
}

int	eventpart(struct s_client *const self, struct s_message const *const cmd)
{
	char	*nick;
	char	*chan;

	nick = NULL;
	chan = NULL;
	nickname(cmd->prefix->data, &nick);
	chan = cmd->params->param[0];
	if (nick && chan)
	{
		self->channels.rmnick(&self->channels, nick, chan);
		printf(" * %s has left %s\n", nick, chan);
	}
	return (0);
}

int	rpl_welcome(struct s_client *const self, struct s_message const *const msg)
{
	(void)self;
	if (msg->params->param[0])
	{
		printf(" * %s\n", msg->params->param[0]);
	}
	return (0);
}

int		client_connect(struct s_client *const self, 
					   struct s_client_msg const *const cmd)
{
	int					portno;
	struct addrinfo		*server = NULL;
	struct addrinfo		hints;

	if (self->servsock > 0)
	{
		LOG(LOGWARN, "Already connected");
		return (-1);
	}
	portno = atoi(cmd->params[1]);

	LOG(LOGDEBUG, "Opening connection to %s:%d", cmd->params[0], portno);

	ft_bzero(&hints, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;

	int ret = 0;
	if ((ret = getaddrinfo(cmd->params[0], cmd->params[1], &hints, &server)) != 0)
	{
		LOG(LOGERR, "getaddrinfo: %s", gai_strerror(ret));
		exit(1);
		return (0);
	}
	if (-1 == (self->servsock = socket(server->ai_family, server->ai_socktype, server->ai_protocol)))
	{
		LOG(LOGERR, "Could not create socket (%s)", strerror(errno));
		exit(EXIT_FAILURE);
	}
	if (connect(self->servsock, server->ai_addr, server->ai_addrlen) < 0)
	{
		LOG(LOGERR, "Could not connect\n");
		printf(" * Could not connect to %s:%d (%s)\n", cmd->params[0], portno, strerror(errno));
	}
	return (0);
}

void	client_del_msg(void *content, size_t size)
{
	free(content);
	(void)size;
}

int		client_sendmsgs(struct s_client *const self)
{
	char	*str;
	int		len;
	int		ret;
	t_list	*next;

	if (self->msgs && FD_ISSET(self->servsock, &self->writefds))
	{
		str = self->msgs->content;
		len = ft_strlen(str);
		ret = send(self->servsock, str, ft_strlen(str), 0);
		LOG(LOGDEBUG, "sending '%s'", str);
		if (ret == len)
		{
			next = self->msgs->next;
			ft_lstdelone(&self->msgs, &client_del_msg);
			self->msgs = next;
		}
		else if (ret > 0)
			ft_memmove(str, str + ret, ft_strlen(str + ret));
		else
		{
			LOG(LOGDEBUG, "%s: could not send message", __FUNCTION__);
			return (-1);
		}
	}
	return (0);
}

int		client_queuemsg(struct s_client *const self, char *msg)
{
	t_list	*elem;

	elem = ft_lstnew(0, 0);
	if (!elem)
		return (-1);
	elem->content = msg;
	elem->content_size = 0;
	ft_lstpush(&self->msgs, elem);
	return (0);
}

int		client_message(struct s_client *const self,
					   struct s_client_msg const *const cmd)
{
	char	*msg;

	if (!self->servsock)
	{
		printf(" * Not connected to a server\n");
		return (0);
	}
	msg = ft_strnew(513);
	if (!msg)
		return (-1);
	ft_strlcat(msg, "PRIVMSG", 513);
	ft_strlcat(msg, " ", 513);
	ft_strlcat(msg, cmd->params[0], 513);
	ft_strlcat(msg, " :", 513);
	ft_strlcat(msg, cmd->params[1], 513);
	ft_strlcat(msg, CRLF, 513);
	return (self->queuemsg(self, msg));
}

int		client_nick(struct s_client *const self,
					struct s_client_msg const *const cmd)
{
	char	*msg;
	char	*params[] = { "TODO", "*", "*", "Todo TODO" };
	struct s_client_msg	usercmd = {
		.cmd = CMD_USER,
		.nparam = 4,
		.params = params,
	};

	if (!self->servsock)
	{
		printf(" * Not connected to a server\n");
		return (0);
	}
	msg = ft_strnew(513);
	if (!msg)
		return (-1);
	ft_strlcat(msg, "NICK", 513);
	ft_strlcat(msg, " ", 513);
	ft_strlcat(msg, cmd->params[0], 513);
	ft_strlcat(msg, CRLF, 513);
	ft_memcpy(self->nickname, cmd->params[0], 9);
	self->queuemsg(self, msg);
	self->user(self, &usercmd);
	return (0);
}

int		client_who(struct s_client *const self,
				   struct s_client_msg const *const cmd)
{
	char	*msg;

	(void)cmd;
	if (!self->servsock)
	{
		printf(" * Not connected to a server\n");
		return (0);
	}
	msg = ft_strnew(513);
	if (!msg)
		return (-1);
	ft_strlcat(msg, "WHO", 513);
//	ft_strlcat(msg, " ", 513);
//	ft_strlcat(msg, cmd->params[0], 513);
	ft_strlcat(msg, CRLF, 513);
	self->queuemsg(self, msg);
	return (0);
}

int		client_execute_command(struct s_client *const self,
							   struct s_client_msg const *const cmd)
{
	const struct s_tuple_cmds cmds[] = {
		{ .cmd = CMD_CONNECT, .f = self->connect },
		{ .cmd = CMD_NICK, .f = self->nick },
		{ .cmd = CMD_JOIN, .f = self->join },
		{ .cmd = CMD_MSG, .f = self->message },
		{ .cmd = CMD_USER, .f = self->user },
		{ .cmd = CMD_LEAVE, .f = self->leave },
		{ .cmd = CMD_WHO, .f = self->who },
	};
	size_t					ii;

	if (!self)
	{
		LOG(LOGWARN, "%s: self == NULL", __FUNCTION__);
		return (-1);
	}
	if (!cmd)
	{
		LOG(LOGWARN, "%s: msg == NULL", __FUNCTION__);
		return (-1);
	}
	ii = 0;
	while (ii < sizeof(cmds) / sizeof(*cmds))
	{
		if (cmds[ii].cmd == cmd->cmd)
			cmds[ii].f(self, cmd);
		ii++;
	}
	return (0);
};

int		eventjoin(struct s_client *const self, struct s_message const *const m)
{
	char				*nick;
	int					ret;
	char const *const	chan = m->params->param[0];

	nick = NULL;
	ret = nickname(m->prefix->data, &nick);
	if (nick)
	{
		LOG(LOGDEBUG, "%s %s", nick, self->nickname);
		if (m->params && m->params->param[0])
		{
			self->channels.addnick(&self->channels, nick, chan);
			if (ft_strnequ(self->nickname, nick, 9))
			{
				self->channel = self->channels.get(&self->channels, m->params->param[0]);
			}
			printf(" * %s joined channel %s\n", nick, chan);
		}
	}
	return (ret);
}

int		eventprivmsg(struct s_client *const self, struct s_message const *const m)
{
	char	*from;
	char	*msg;
	char	*channel;

	(void)self;
	from = m->prefix->data;
	channel = m->params->param[0];
	msg = m->params->param[1];
	printf("[%s] %s: %s\n", channel, from, msg);
	return (0);
}

int	eventping(struct s_client *const self, struct s_message const *const msg)
{
	(void)msg;
	self->queuemsg(self, ft_strdup("PONG\x0d\x0a"));
	return (0);
}

int		client_event(struct s_client *self, char const *const data)
{
	struct s_event_list events[] = {
		{ .s = "JOIN", .f = self->eventjoin },
		{ .s = "PRIVMSG", .f = self->eventprivmsg },
		{ .s = "PART", .f = self->eventpart },
		{ .s = "PING", .f = self->eventping },
		{ .s = "001", .f = self->rpl_welcome },
		{ .s = "372", .f = self->eventmotd },
		{ .s = "375", .f = self->eventmotd },
		{ .s = "376", .f = self->eventmotd },
	};
	size_t				ii;

	struct s_message	*msg;

	msg = message(data);
	if (!msg)
		return (0);
	ii = 0;
	while (ii < sizeof(events) / sizeof(*events))
	{
		if (ft_strequ(events[ii].s, msg->cmd->data))
			events[ii].f(self, msg);
		ii += 1;
	}
	return (0);
}

int		client_join(struct s_client *const self,
					struct s_client_msg const *const cmd)
{
	char	*msg;

	if (!self->servsock)
	{
		printf(" * Not connected to a server\n");
		return (0);
	}
	msg = ft_strnew(513);
	if (!msg)
		return (-1);
	ft_strlcat(msg, "JOIN", 513);
	ft_strlcat(msg, " ", 513);
	ft_strlcat(msg, cmd->params[0], 513);
	ft_strlcat(msg, CRLF, 513);
	return (self->queuemsg(self, msg));
}

int		client_user(struct s_client *const self,
					struct s_client_msg const *const cmd)
{
	char	*msg;

	if (!self->servsock)
	{
		printf(" * Not connected to a server\n");
		return (0);
	}
	msg = ft_strnew(513);
	if (!msg)
		return (-1);
	ft_strlcat(msg, "USER", 513);
	ft_strlcat(msg, " ", 513);
	ft_strlcat(msg, cmd->params[0], 513);
	ft_strlcat(msg, " ", 513);
	ft_strlcat(msg, cmd->params[1], 513);
	ft_strlcat(msg, " ", 513);
	ft_strlcat(msg, cmd->params[2], 513);
	ft_strlcat(msg, " :", 513);
	ft_strlcat(msg, cmd->params[3], 513);
	ft_strlcat(msg, CRLF, 513);
	return (self->queuemsg(self, msg));
}

int		client_leave(struct s_client *const self,
					 struct s_client_msg const *const cmd)
{
	char	*msg;

	if (!self->servsock)
	{
		printf(" * Not connected to a server\n");
		return (0);
	}
	msg = ft_strnew(513);
	if (!msg)
		return (-1);
	ft_strlcat(msg, "PART", 513);
	ft_strlcat(msg, " ", 513);
	ft_strlcat(msg, cmd->params[0], 513);
	ft_strlcat(msg, CRLF, 513);
	self->channel = 0;
	return (self->queuemsg(self, msg));
}

void	client_init(struct s_client *self)
{
	if (!self)
		return ;
	self->servsock = 0;
	self->channel = 0;
	self->msgs = NULL;
	ft_memset(self->nickname, 0, 9);
	channels_init(&self->channels);
	self->run = &client_run;
	self->exec_cmd = &client_execute_command;
	self->queuemsg = &client_queuemsg;
	self->sendmsgs = &client_sendmsgs;
	self->event = &client_event;
	self->parse_input = &parse_input;
	self->eventping = &eventping;

	self->connect = &client_connect;
	self->message = &client_message;
	self->nick = &client_nick;
	self->join = &client_join;
	self->user = &client_user;
	self->leave = &client_leave;
	self->who = &client_who;

	self->eventjoin = &eventjoin;

	self->rpl_welcome = &rpl_welcome;
	self->eventprivmsg = &eventprivmsg;
	self->eventpart = &eventpart;
	self->eventmotd = &eventmotd;

	self->raw_buffer = malloc(sizeof(char) * 2048);
	self->cbuf = cbuf_init(self->raw_buffer, 2048);
}

int	main(void)
{
	struct s_client		client;

	client_init(&client);
	client.run(&client);
	close(client.servsock);
	return (0);
}
