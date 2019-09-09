/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server_queue.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oyagci <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/09/09 14:27:37 by oyagci            #+#    #+#             */
/*   Updated: 2019/09/09 14:27:41 by oyagci           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "server.h"
#include <sys/socket.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <cbuf.h>

void		server_msg_del(void *msgp, size_t size)
{
	struct s_server_msg *msg;

	(void)size;
	msg = msgp;
	free(msg);
}

int			send_queued_replies(struct s_server *const server)
{
	t_list				*prev;
	t_list				*next;
	t_list				*msgelem;
	struct s_server_msg	*msg;
	struct s_client		*dest;

	prev = NULL;
	next = NULL;
	msgelem = server->msgqueue;
	while (msgelem)
	{
		next = msgelem->next;
		msg = msgelem->content;
		dest = server->get_client(server, msg->dest);
		if (dest && FD_ISSET(dest->fd, &server->writefds))
		{
			send(dest->fd, msg->msg, msg->len, 0);
			prev ? (prev->next = msgelem->next) :
				(server->msgqueue = msgelem->next);
			ft_lstdelone(&msgelem, &server_msg_del);
		}
		prev = msgelem;
		msgelem = next;
	}
	return (0);
}

static char	*server_format_reply(struct s_client const *const c, int reply_code)
{
	t_rpl_handle const	handles[] = { rpl_welcome, err_unknowncmd,
		err_nickinuse, err_erroneusnick };
	int const			replies[] = { RPL_WELCOME, ERR_UNKNOWNCOMMAND,
		ERR_NICKNAMEINUSE, ERR_ERRONEUSNICKNAME };
	char				*reply;
	size_t				i;

	reply = NULL;
	i = 0;
	if (reply_code > 0)
	{
		reply = ft_strnew(512);
		while (i < sizeof(handles) / sizeof(*handles))
		{
			if (replies[i] == reply_code)
				handles[i](reply, 512, c);
			i += 1;
		}
	}
	return (reply);
}

int			queue_reply(struct s_server *server,
				struct s_client const *const dest, char *reply)
{
	t_list				*elem;
	struct s_server_msg	*msg;

	if (!(msg = ft_memalloc(sizeof(*msg))))
	{
		exit(EXIT_FAILURE);
		return (-1);
	}
	ft_strlcpy(msg->dest, dest->nickname, NICK_SIZE);
	ft_strlcpy(msg->msg, reply, 512);
	msg->len = ft_strlen(msg->msg);
	elem = ft_lstnew(0, 0);
	elem->content = msg;
	ft_lstadd(&server->msgqueue, elem);
	return (0);
}

int			queue_code_reply(struct s_server *server,
	struct s_client const *const dest, int reply_code)
{
	char				*replystr;

	replystr = server_format_reply(dest, reply_code);
	if (!replystr)
		return (-1);
	server->queuenotif(server, dest, replystr);
	free(replystr);
	return (0);
}
