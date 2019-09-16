/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   accept_new_clients.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oyagci <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/09/09 14:27:36 by oyagci            #+#    #+#             */
/*   Updated: 2019/09/09 14:27:40 by oyagci           ###   ########.fr       */
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

int		setnosigpipe(void)
{
	signal(SIGPIPE, SIG_IGN);
	return (0);
}

void	add_client(struct s_server *self, struct s_client *c)
{
	t_list	*elem;

	elem = ft_lstnew(NULL, 0);
	if (!elem)
		exit(EXIT_FAILURE);
	elem->content = c;
	ft_lstadd(&self->clients, elem);
}

int		accept_new_clients(struct s_server *server)
{
	struct s_client			*client;
	struct sockaddr_in		cli_addr;
	socklen_t				cli_len;
	int						confd;

	cli_len = sizeof(cli_addr);
	if (FD_ISSET(server->sockfd, &server->readfds))
	{
		confd = accept(server->sockfd, (struct sockaddr *)&cli_addr, &cli_len);
		setnosigpipe();
		if (!(client = ft_memalloc(sizeof(*client))))
			exit(EXIT_FAILURE);
		client->fd = confd;
		client->server = server;
		ft_memcpy(client->nickname, "", 1);
		if (!(client->raw_buffer = ft_memalloc(sizeof(char) * 2048)))
			exit(EXIT_FAILURE);
		client->cbuf = cbuf_init(client->raw_buffer, 2048);
		add_client(server, client);
	}
	return (0);
}
