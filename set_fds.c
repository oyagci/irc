/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   set_fds.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oyagci <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/05 15:49:23 by oyagci            #+#    #+#             */
/*   Updated: 2019/08/05 16:25:16 by oyagci           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "irc.h"
#include <sys/select.h>
#include <stdio.h>

int		set_fds(int sockfd, t_list *clients, fd_set *readfds, fd_set *writefds)
{
	t_list			*cur;
	struct s_client	*client;
	int				max_fd;

	FD_ZERO(readfds);
	FD_SET(sockfd, readfds);
	FD_ZERO(writefds);
	FD_SET(sockfd, writefds);
	max_fd = sockfd;
	cur = clients;
	while (cur)
	{
		client = cur->content;
		FD_SET(client->fd, readfds);
		FD_SET(client->fd, writefds);
		if (client->fd > max_fd)
			max_fd = client->fd;
		cur = cur->next;
	}
	return (max_fd);
}
