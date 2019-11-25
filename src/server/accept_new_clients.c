/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   accept_new_clients.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oyagci <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/09/09 14:27:36 by oyagci            #+#    #+#             */
/*   Updated: 2019/11/25 11:33:28 by oyagci           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "server.h"
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <time.h>
#include <stdlib.h>
#include <unistd.h>

static int		get_client_slot(t_server *self)
{
	size_t	i;

	i = 0;
	while (i < NCLIENTS)
	{
		if (self->clients[i].fd == 0)
			return (i);
		i += 1;
	}
	return (-1);
}

static int		add_new_client(t_server *server, int fd)
{
	int			slot;
	t_client	*client;
	
	slot = get_client_slot(server);
	if (slot < 0)
		return (-1);
	ft_memset(server->clients+ slot, 0, sizeof(*client));
	client = server->clients + slot;
	client->fd = fd;
	client->server = server;
	client->cbuf = cbuf_init(client->raw_buffer, sizeof(client->raw_buffer));
	client->reply = cbuf_init(client->replybuf, sizeof(client->replybuf));
	return (0);
}

int				accept_new_clients(struct s_server *server)
{
	struct sockaddr_in	cli_addr;
	socklen_t			cli_len;
	int					confd;

	cli_len = sizeof(cli_addr);
	if (!FD_ISSET(server->sockfd, &server->readfds))
		return (0);
	confd = accept(server->sockfd, (struct sockaddr *)&cli_addr, &cli_len);
	if (confd < 0)
		return (-1);
	if (add_new_client(server, confd) < 0)
	{
		close(confd);
		return (0);
	}
	return (0);
}
