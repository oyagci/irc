/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   read_client_commands.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oyagci <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/09/09 14:27:36 by oyagci            #+#    #+#             */
/*   Updated: 2019/09/09 14:27:40 by oyagci           ###   ########.fr       */
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
static ssize_t read_to_buffer(int cfd, t_cbuf_handle cbuf)
{
	int ret;
	char buf[512];
	int ii;
	int complete;

	ft_memset(buf, 0, 512);
	complete = 0;
	ret = recv(cfd, buf, 512, 0);
	if (ret > 0) {
		complete = 0;

		ii = 0;
		while (ii < ret) {
			cbuf_put(cbuf, buf[ii]);
			if (buf[ii] == '\n') {
				complete += 1;
			}
			ii += 1;
		}
	}
	else if (ret == 0) {
		/* TODO: Client is disconnected */
	}
	else {
		perror("recv");
		return (-1);
	}
	return (complete);
}

static void	do_command(struct s_server *self, struct s_client *client)
{
	char			buf[512];
	unsigned char	data;
	int				ii;

	ft_memset(buf, 0, 512);
	ii = 0;
	while (ii < 512)
	{
		if (cbuf_get(client->cbuf, &data) < 0)
			break ;
		buf[ii] = data;
		if (buf[ii] == '\n')
			break ;
		ii += 1;
	}
	self->exec_cmd(self, client, buf);
}

int			read_clients_command(struct s_server *const self)
{
	t_list			*next;
	t_list			*cur;
	struct s_client	*client;
	int				complete;
	int				ret = 0;

	cur = self->clients;
	while (cur)
	{
		next = cur->next;
		client = cur->content;
		complete = read_to_buffer(client->fd, client->cbuf);
		if (complete < 0) {
			ret = -1;
			break ;
		}
		while (complete > 0) {
			complete -= 1;
			do_command(self, client);
		}
		cur = next;
	}
	return (ret);
}
