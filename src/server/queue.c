/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server_queue.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oyagci <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/09/09 14:27:37 by oyagci            #+#    #+#             */
/*   Updated: 2019/11/22 13:52:33 by oyagci           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "server.h"
#include <sys/socket.h>
#include <unistd.h>
#include <stdlib.h>
#include <cbuf.h>

void		send_queued_replie(t_client *dest)
{
	int		ret;
	size_t	i;
	uint8_t	buf[512];

	i = 0;
	while (cbuf_get(dest->reply, &buf[i]) && i < 512)
		i += 1;
	ret = send(dest->fd, buf, i, 0);
	if (ret == (int)i)
		dest->nmsg -= 1;
	else if (ret <= 0)
		dest->should_be_disconnected = 1;
}

int			send_queued_replies(struct s_server *const server)
{
	size_t	i;

	i = 0;
	while (i < NCLIENTS)
	{
		if (server->clients[i].fd != 0)
			send_queued_replie(&server->clients[i]);
		i += 1;
	}
	return (0);
}

static void	server_format_reply(t_client const *const c, int reply_code,
	char buf[512])
{
	t_tuple_reply const	handles[] = {
		{ RPL_WELCOME, rpl_welcome },
		{ ERR_UNKNOWNCOMMAND, err_unknowncmd },
		{ ERR_NICKNAMEINUSE, err_nickinuse },
		{ ERR_ERRONEUSNICKNAME, err_erroneusnick },
	};
	size_t				i;

	if (reply_code > 0)
	{
		i = 0;
		while (i < sizeof(handles) / sizeof(*handles))
		{
			if (handles[i].num == reply_code)
			{
				handles[i].f((char *)buf, 512, c);
				break ;
			}
			i += 1;
		}
	}
}

int			queue_reply(t_server *server, t_client *const dest,
	char const *reply)
{
	size_t	i;

	(void)server;
	i = 0;
	while (reply[i] != 0)
	{
		cbuf_put(dest->reply, reply[i]);
		i += 1;
	}
	dest->nmsg += 1;
	return (0);
}

int			queue_code_reply(t_server *server, t_client *const dest,
	int reply_code)
{
	char replystr[512];

	ft_memset(replystr, 0, sizeof(replystr));
	server_format_reply(dest, reply_code, replystr);
	return (queue_reply(server, dest, replystr));
}
