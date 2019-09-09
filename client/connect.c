/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   connect.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oyagci <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/09/09 13:04:48 by oyagci            #+#    #+#             */
/*   Updated: 2019/09/09 13:04:52 by oyagci           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "client.h"
#include "logger.h"

#include <sys/types.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

void		catnick(char *s, char const *nick, size_t slen, size_t nlen)
{
	size_t	i;
	size_t	j;

	i = 0;
	while (s[i] != 0 && i < slen)
		i++;
	s += i;
	if (slen - i >= nlen + 1)
	{
		j = 0;
		while (j < nlen && j + i < slen)
		{
			s[j] = nick[j];
			j++;
		}
		s[j] = 0;
	}
}

static void	set_usercmd(struct s_client_msg *msg)
{
	msg->cmd = CMD_USER;
	msg->nparam = 4;
	ft_strcpy(msg->params[0], "TODO");
	ft_strcpy(msg->params[0], "*");
	ft_strcpy(msg->params[0], "*");
	ft_strcpy(msg->params[0], "Todo TODO");
}

static int	register_conn(struct s_client *const self)
{
	char				*msg;
	struct s_client_msg usercmd;

	set_usercmd(&usercmd);
	msg = ft_strnew(513);
	if (!msg)
		return (-1);
	ft_strlcat(msg, "NICK", 513);
	ft_strlcat(msg, " ", 513);
	catnick(msg, self->nickname, 513, SNICK);
	ft_strlcat(msg, CRLF, 513);
	self->queuemsg(self, msg);
	self->user(self, &usercmd);
	self->is_registered = 1;
	return (0);
}

static int	do_conn(struct s_client *self, char const *addr,
	char const *portstr, int portno)
{
	struct addrinfo		*server;
	struct addrinfo		hints;
	int					ret;
	int					gaierr;

	server = NULL;
	ret = -1;
	printf(" - Connecting to %s:%d\n", addr, portno);
	ft_bzero(&hints, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	if ((gaierr = getaddrinfo(addr, portstr, &hints, &server)) != 0)
		LOG(LOGERR, "getaddrinfo: %s", gai_strerror(gaierr));
	else if (-1 == (self->servsock = socket(server->ai_family,
				server->ai_socktype, server->ai_protocol)))
		LOG(LOGERR, "Could not create socket (%s)", strerror(errno));
	else if (connect(self->servsock, server->ai_addr, server->ai_addrlen) < 0)
		printf(" - Could not connect to %s:%d\n", addr, portno);
	else
	{
		printf(" - Connected to %s\n", addr);
		ret = 0;
	}
	freeaddrinfo(server);
	return (ret);
}

int			client_connect(struct s_client *const self,
	struct s_client_msg const *const cmd)
{
	unsigned short	portno;
	int				ret;

	if (self->nickname[0] == '\0')
	{
		printf(" - Please set your nickname first\n");
		return (-1);
	}
	if (self->servsock > 0)
	{
		printf(" - Sorry, already connected\n");
		return (-1);
	}
	portno = ft_atoi(cmd->params[1]);
	ret = do_conn(self, cmd->params[0], cmd->params[1], portno);
	if (ret >= 0)
		register_conn(self);
	return (ret);
}
