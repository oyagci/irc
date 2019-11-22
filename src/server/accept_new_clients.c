/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   accept_new_clients.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oyagci <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/09/09 14:27:36 by oyagci            #+#    #+#             */
/*   Updated: 2019/11/22 13:16:49 by oyagci           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "server.h"
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <time.h>
#include <signal.h>
#include <stdlib.h>
#include <unistd.h>
#include "log.h"

static int setnosigpipe(void)
{
	signal(SIGPIPE, SIG_IGN);
	return (0);
}

static int		add_client(struct s_server *self, t_client *c)
{
	size_t	i;

	i = 0;
	while (i < NCLIENTS)
	{
		if (self->clients[i].fd == 0)
		{
			ft_memcpy(self->clients + i, c, sizeof(t_client));
			return (0);
		}
		i += 1;
	}
	return (-1);
}

void	client_init(t_client *client, struct s_server *s, int fd)
{
	ft_memset(client, 0, sizeof(t_client));
	client->fd = fd;
	client->server = s;
	client->cbuf = cbuf_init(client->raw_buffer, sizeof(client->raw_buffer));
	ft_memset(client->nickname, 0, sizeof(client->nickname));
}

int				accept_new_clients(struct s_server *server)
{
	t_client			client;
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
	setnosigpipe();
	client_init(&client, server, confd);
	if (add_client(server, &client) < 0)
	{
		ERROR("Could not add client to list. Closing connection...");
		close(confd);
		return (0);
	}
	return (0);
}
