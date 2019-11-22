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
	t_list *prev;
	t_list *next;
	t_list *msgelem;
	struct s_server_msg *msg;
	struct s_client *dest;
	int ret = 0;

	prev = NULL;
	msgelem = server->msgqueue;
	while (NULL != msgelem)
	{
		next = msgelem->next;
		msg = msgelem->content;
		dest = get_client(server, msg->dest);
		if (NULL != dest)
		{
			ret = send(dest->fd, msg->msg, msg->len, 0);
			if (ret == (int)msg->len) {
				/* Remove message from queue */
				dest->nmsg -= 1;
				if (prev) {
					prev->next = msgelem->next;
				}
				else {
					server->msgqueue = msgelem->next;
				}
				ft_lstdelone(&msgelem, &server_msg_del);
			}
			else if (ret > 0) {
				/* Remove only sent bytes */
				ft_memcpy(msg->msg, msg->msg + ret, msg->len - ret);
				msg->msg[ret] = 0;
			}
			else if (ret == 0) {
				/* Mark client as disconnected */
				/* TODO: HANDLE ACTUAL DISCONNECTION */
				dest->should_be_disconnected = 1;
			}
			else if (ret == -1) {
				dest->should_be_disconnected = 1;
				perror("send");
			}
		}
		prev = msgelem;
		msgelem = next;
	}
	return (0);
}

static void server_format_reply(struct s_client const *const c, int reply_code,
	char buf[512])
{
	struct {
		int num;
		int (*f)(char *, size_t, struct s_client const *);
	} const handles[] = {
		{ RPL_WELCOME,          rpl_welcome },
		{ ERR_UNKNOWNCOMMAND,   err_unknowncmd },
		{ ERR_NICKNAMEINUSE,    err_nickinuse },
		{ ERR_ERRONEUSNICKNAME,	err_erroneusnick },
	};

	if (reply_code > 0) {
		for (size_t i = 0; i < sizeof(handles) / sizeof(*handles); i++) {
			if (handles[i].num == reply_code) {
				handles[i].f((char *)buf, 512, c);
				break;
			}
		}
	}
}

int queue_reply(struct s_server *server, struct s_client *const dest,
	char const *reply)
{
	t_list *elem;
	struct s_server_msg *msg;

	msg = ft_memalloc(sizeof(*msg));
	if (!msg) {
		perror("malloc");
		return (-1);
	}
	dest->nmsg += 1;
	ft_strlcpy(msg->dest, dest->nickname, NICK_SIZE);
	ft_strlcpy(msg->msg, reply, 512);
	msg->len = ft_strlen(msg->msg);

	/* Add reply to the queue */
	elem = ft_lstnew(0, 0);
	elem->content = msg;
	ft_lstadd(&server->msgqueue, elem);

	return (0);
}

int queue_code_reply(struct s_server *server, struct s_client *const dest,
	int reply_code)
{
	char replystr[512];

	ft_memset(replystr, 0, sizeof(replystr));
	server_format_reply(dest, reply_code, replystr);
	return (queue_reply(server, dest, replystr));
}
