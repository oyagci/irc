/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   read_client_commands.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oyagci <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/09/09 14:27:36 by oyagci            #+#    #+#             */
/*   Updated: 2019/11/22 13:16:58 by oyagci           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "server.h"
#include <sys/socket.h>
#include <stdio.h>

/*
 * Read from a file descriptor at most 512 bytes
 * and put them into a circular buffer
 *
 * Returns the number of '\n' terminated commands
 */
static int read_to_buffer(int cfd, t_cbuf_handle cbuf)
{
	int ret;
	char buf[512];
	int ii;
	int complete;

	ft_memset(buf, 0, 512);
	complete = 0;
	ret = recv(cfd, buf, 512, 0);
	if (ret > 0)
	{
		ii = 0;
		while (ii < ret)
		{
			cbuf_put(cbuf, buf[ii]);
			buf[ii] == '\n' ? complete++ : 0;
			ii += 1;
		}
	}
	else if (ret == 0)
		return (-1);
	else
	{
		perror("recv");
		return (-1);
	}
	return (complete);
}

/*
 * Read commands read from client input
 * Returns -1 on error or the number of commands read.
 */
int			read_client_command(struct s_server *const self)
{
	int		complete;
	size_t	i;
	
	i = 0;
	while (i < NCLIENTS)
	{
		if (self->clients[i].fd != 0
			&& FD_ISSET(self->clients[i].fd, &self->readfds))
		{
			complete = read_to_buffer(self->clients[i].fd,
				self->clients[i].cbuf);
			self->clients[i].ncmds = complete;
			if (complete < 0)
				self->clients[i].should_be_disconnected = 1;
		}
		i += 1;
	}
	return (0);
}
