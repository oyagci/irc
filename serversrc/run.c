/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server_loop.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oyagci <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/09/09 14:27:37 by oyagci            #+#    #+#             */
/*   Updated: 2019/09/09 14:27:41 by oyagci           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "server.h"

/*
 * Set file descriptors for reading on each client
 * Also set file descriptors for writing on clients with messages queued
 */
static int set_fds(struct s_server *server)
{
	int max_fd = 0;

	FD_ZERO(&server->readfds);
	FD_SET(server->sockfd, &server->readfds);
	FD_ZERO(&server->writefds);
	FD_SET(server->sockfd, &server->writefds);

	max_fd = server->sockfd;

	for (t_list *cur = server->clients; cur != NULL; cur = cur->next) {

		struct s_client	*client = cur->content;

		FD_SET(client->fd, &server->readfds);

		/*
		 * Only set file descriptors for writing if a message is about to be
		 * sent to that specific client
		 */
		if (client->nmsg > 0)
			FD_SET(client->fd, &server->writefds);

		if (client->fd > max_fd)
			max_fd = client->fd;
	}
	return (max_fd);
}

/*
 * Main loop
 *
 * This function:
 *   - Accepts new clients
 *   - Reads client messages
 *   - Replies to clients
 */
int run(struct s_server *self)
{
	int max_sd;

	self->is_running = 1;
	while (self->is_running) {
		max_sd = set_fds(self);
		select(max_sd + 1, &self->readfds, &self->writefds, NULL, NULL);
		self->accept(self);
		self->read(self);
		self->send(self);
		self->update_clients(self);
	}
	return (0);
}
