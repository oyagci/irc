/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server_loop.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oyagci <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/09/09 14:27:37 by oyagci            #+#    #+#             */
/*   Updated: 2019/11/22 13:48:39 by oyagci           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "server.h"
#include "log.h"

static int	set_fds(struct s_server *server)
{
	int			max_fd;
	t_client	*client;
	size_t		i;

	max_fd = server->sockfd;
	FD_ZERO(&server->readfds);
	FD_SET(server->sockfd, &server->readfds);
	FD_ZERO(&server->writefds);
	FD_SET(server->sockfd, &server->writefds);
	i = 0;
	while (i < NCLIENTS)
	{
		client = server->clients + i;
		if (client->fd > 0)
		{
			FD_SET(client->fd, &server->readfds);
			if (client->nmsg > 0)
				FD_SET(client->fd, &server->writefds);
			if (client->fd > max_fd)
				max_fd = client->fd;
		}
		i += 1;
	}
	return (max_fd);
}

int			run(struct s_server *self)
{
	int max_sd;

	VERBOSE("Running main loop");
	self->is_running = 1;
	while (self->is_running)
	{
		max_sd = set_fds(self);
		select(max_sd + 1, &self->readfds, &self->writefds, NULL, NULL);
		if (accept_new_clients(self) < 0)
		{
			; // TODO: Handle error
		}
		if (read_client_command(self) < 0)
		{
			; // TODO: Handler error
		}
		docommands(self);
		send_queued_replies(self);
		update_clients(self);
	}
	return (0);
}
