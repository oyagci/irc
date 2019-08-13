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
		printf(" * %s left %s\n", nick, chan);
	}
	return (0);
}

int	rpl_welcome(struct s_client *const self, struct s_message const *const msg)
{
	LOG(LOGDEBUG, "rpl_welcome");
	(void)self;
	if (msg->params->param[0])
	{
		printf(" * %s\n", msg->params->param[0]);
	}
	return (0);
}

int		client_connect(struct s_client *const self, struct s_client_msg const *const cmd)
{
	int					portno;
	struct sockaddr_in	serv_addr;

	if (self->servsock > 0)
	{
		LOG(LOGWARN, "Already connected");
		return (-1);
	}
	portno = atoi(cmd->params[1]);

	LOG(LOGDEBUG, "Opening connection to %s:%d\n", cmd->params[0], portno);

	ft_memset(&serv_addr, 0, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(portno);
	if (-1 == (self->servsock = socket(AF_INET, SOCK_STREAM, 0)))
	{
		LOG(LOGERR, "Could not create socket (%s)", strerror(errno));
		exit(EXIT_FAILURE);
	}
	if (connect(self->servsock, (struct sockaddr *)&serv_addr,
				sizeof(serv_addr)) < 0)
	{
		LOG(LOGERR, "Could not connect\n");
		exit(EXIT_FAILURE);
	}
	return (-1);
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

int		client_message(struct s_client *const self, struct s_client_msg const *const cmd)
{
	char	*msg;

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

int		client_nick(struct s_client *const self, struct s_client_msg const *const cmd)
{
	char	*msg;
	char	*params[] = { "TODO", "*", "*", "Todo TODO" };
	struct s_client_msg	usercmd = {
		.cmd = CMD_USER,
		.nparam = 4,
		.params = params,
	};

	msg = ft_strnew(513);
	if (!msg)
		return (-1);
	ft_strlcat(msg, "NICK", 513);
	ft_strlcat(msg, " ", 513);
	ft_strlcat(msg, cmd->params[0], 513);
	ft_strlcat(msg, CRLF, 513);
	self->queuemsg(self, msg);
	self->user(self, &usercmd);
	return (0);
}

int		client_execute_command(struct s_client *const self, struct s_client_msg const *const cmd)
{
	const struct s_tuple_cmds cmds[] = {
		{ .cmd = CMD_CONNECT, .f = self->connect },
		{ .cmd = CMD_NICK, .f = self->nick },
		{ .cmd = CMD_JOIN, .f = self->join },
		{ .cmd = CMD_MSG, .f = self->message },
		{ .cmd = CMD_USER, .f = self->user },
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
		if (m->params && m->params->param[0])
		{
			self->channels.addnick(&self->channels, nick, chan);
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

int		client_event(struct s_client *self, char const *const data)
{
	struct s_event_list events[] = {
		{ .s = "JOIN", .f = self->eventjoin },
		{ .s = "PRIVMSG", .f = self->eventprivmsg },
		{ .s = "PART", .f = self->eventpart },
		{ .s = "001", .f = self->rpl_welcome },
	};
	size_t				ii;

	struct s_message	*msg;

	msg = message(data);
	if (!msg)
		return (0);
	ii = 0;
	LOG(LOGDEBUG, "event: %s", msg->cmd->data);
	while (ii < sizeof(events) / sizeof(*events))
	{
		if (ft_strequ(events[ii].s, msg->cmd->data))
		{
			LOG(LOGDEBUG, "%s", events[ii].s);
			events[ii].f(self, msg);
		}
		ii += 1;
	}
	return (0);
}

int		client_run(struct s_client *self)
{
	struct s_client_msg	*cmd;
	char				*c;
	char				buf[512];
	struct timeval		t;

	self->is_running = 1;
	while (self->is_running)
	{
		FD_ZERO(&self->readfds);
		FD_SET(self->servsock, &self->readfds);
		FD_SET(STDIN_FILENO, &self->readfds);
		FD_ZERO(&self->writefds);
		FD_SET(self->servsock, &self->writefds);
		t.tv_sec = 0;
		t.tv_usec = 10000;
		select(self->servsock + 1, &self->readfds, NULL, NULL, &t);
		if (FD_ISSET(self->servsock, &self->readfds))
		{
			ft_memset(buf, 0, 512);
			int	ret = recv(self->servsock, buf, 512, 0);
			if (ret > 0)
			{
				self->event(self, buf);
				char *c = ft_strstr(buf, CRLF);
				if (c)
					*c = '\0';
				LOG(LOGDEBUG, "%s", buf);
			}
		}
		if (FD_ISSET(0, &self->readfds))
		{
			fgets(buf, 512, stdin);
			if ((c = ft_strchr(buf, '\n')))
				*c = '\0';
			cmd = self->parse_input(buf);
			self->exec_cmd(self, cmd);
		}
		self->sendmsgs(self);
	}
	return (0);
}

int		client_join(struct s_client *const self, struct s_client_msg const *const cmd)
{
	char	*msg;

	msg = ft_strnew(513);
	if (!msg)
		return (-1);
	ft_strlcat(msg, "JOIN", 513);
	ft_strlcat(msg, " ", 513);
	ft_strlcat(msg, cmd->params[0], 513);
	ft_strlcat(msg, CRLF, 513);
	return (self->queuemsg(self, msg));
}

int		client_user(struct s_client *const self, struct s_client_msg const *const cmd)
{
	char	*msg;

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

void	client_init(struct s_client *self)
{
	if (!self)
		return ;
	self->servsock = 0;
	self->msgs = NULL;
	channels_init(&self->channels);
	self->run = &client_run;
	self->exec_cmd = &client_execute_command;
	self->queuemsg = &client_queuemsg;
	self->sendmsgs = &client_sendmsgs;
	self->event = &client_event;
	self->parse_input = &parse_input;

	self->connect = &client_connect;
	self->message = &client_message;
	self->nick = &client_nick;
	self->join = &client_join;
	self->user = &client_user;

	self->eventjoin = &eventjoin;

	self->rpl_welcome = &rpl_welcome;
	self->eventprivmsg = &eventprivmsg;
	self->eventpart = &eventpart;
}

int	main(void)
{
	struct s_client		client;

	client_init(&client);
	client.run(&client);
	close(client.servsock);
	return (0);
}
