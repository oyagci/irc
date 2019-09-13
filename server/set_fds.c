/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   set_fds.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oyagci <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/05 15:49:23 by oyagci            #+#    #+#             */
/*   Updated: 2019/09/09 14:27:41 by oyagci           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "server.h"
#include <sys/select.h>
#include <stdio.h>

int		set_fds(struct s_server *server)
{
	t_list			*cur;
	struct s_client	*client;
	int				max_fd;

	FD_ZERO(&server->readfds);
	FD_SET(server->sockfd, &server->readfds);
	FD_ZERO(&server->writefds);
	FD_SET(server->sockfd, &server->writefds);
	max_fd = server->sockfd;
	cur = server->clients;
	while (cur)
	{
		client = cur->content;
		FD_SET(client->fd, &server->readfds);
		if (client->nmsg > 0)
			FD_SET(client->fd, &server->writefds);
		if (client->fd > max_fd)
			max_fd = client->fd;
		cur = cur->next;
	}
	return (max_fd);
}
