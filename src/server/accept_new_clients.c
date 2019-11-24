/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   accept_new_clients.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oyagci <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/09/09 14:27:36 by oyagci            #+#    #+#             */
/*   Updated: 2019/11/22 13:53:32 by oyagci           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "server.h"
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <unistd.h>
#include "log.h"

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
	client = server->clients + slot;
	client->fd = fd;
	client->server = server;
	client->cbuf = cbuf_init(client->raw_buffer, sizeof(client->raw_buffer));
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
	{
		perror("accept");
		return (-1);
	}
	if (add_new_client(server, confd) < 0)
	{
		ERROR("Could not add client to list. Closing connection...");
		close(confd);
		return (0);
	}
	return (0);
}
