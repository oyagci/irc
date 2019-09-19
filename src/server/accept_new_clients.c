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
#include <unistd.h>

static int setnosigpipe(void)
{
	signal(SIGPIPE, SIG_IGN);
	return (0);
}

static int add_client(struct s_server *self, struct s_client *c)
{
	t_list	*elem;

	if (!c) {
		return (-1);
	}

	elem = ft_lstnew(NULL, 0);
	if (!elem) {
		return (-1);
	}
	elem->content = c;
	ft_lstadd(&self->clients, elem);
	return (0);
}

int accept_new_clients(struct s_server *server)
{
	struct s_client *client;
	struct sockaddr_in cli_addr;
	socklen_t cli_len;
	int confd;
	int ret = 0;

	cli_len = sizeof(cli_addr);
	/* Test if a new connection happened */
	if (FD_ISSET(server->sockfd, &server->readfds)) {
		confd = accept(server->sockfd, (struct sockaddr *)&cli_addr, &cli_len);
		if (confd >= 0) {

			setnosigpipe();

			client = ft_memalloc(sizeof(*client));
			if (client) {
				client->fd = confd;
				client->server = server;
				client->cbuf = cbuf_init(client->raw_buffer, sizeof(client->raw_buffer));
				ft_memset(client->nickname, 0, sizeof(client->nickname));

				if (!add_client(server, client)) {
					printf("New client connected\n");
				}
				else {
					fprintf(stderr,
						"Could not add client to list. Closing connection...\n");
					close(confd);
					ret = -1;
				}
			}
			else {
				perror("malloc");
				ret = -1;
			}
		}
		else {
			perror("accept");
			ret = -1;
		}
	}
	return (ret);
}
