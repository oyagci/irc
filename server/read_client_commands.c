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

static int	read_to_buffer(int cfd, t_cbuf_handle cbuf)
{
	int		ret;
	char	buf[512];
	int		ii;
	int		complete;

	ft_memset(buf, 0, 512);
	complete = 0;
	if ((ret = recv(cfd, buf, 512, 0)) > 0)
	{
		complete = 0;
		ii = 0;
		while (ii < ret)
		{
			if (cbuf_put2(cbuf, buf[ii]) < 0)
				break ;
			if (buf[ii] == '\n')
				complete += 1;
			ii += 1;
		}
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

	cur = self->clients;
	while (cur)
	{
		next = cur->next;
		client = cur->content;
		if (FD_ISSET(client->fd, &self->readfds))
		{
			complete = read_to_buffer(client->fd, client->cbuf);
			while (complete)
			{
				complete -= 1;
				do_command(self, client);
			}
		}
		cur = next;
	}
	return (0);
}
