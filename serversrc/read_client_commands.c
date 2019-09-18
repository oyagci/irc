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

static int do_command(struct s_server *self, struct s_client *client)
{
	char buf[512] = { 0 };
	unsigned char data = 0;
	int ii = 0;

	ft_memset(buf, 0, 512);
	ii = 0;
	while (ii < 512)
	{
		if (cbuf_get(client->cbuf, &data) < 0) {
			/* No more data, command is complete */
			break ;
		}

		buf[ii] = data;

		if (buf[ii] == '\n') {
			/* A newline means command is complete */
			break ;
		}
		ii += 1;
	}
	return (self->exec_cmd(self, client, buf));
}

/*
 * Read and execute commands read from client input
 * Returns -1 on error or the number of commands read.
 *
 * FIXME: This function should only READ commands and put them inside a buffer,
 *        execution should be done elsewhere
 */
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
		ret = complete;
		while (complete > 0) {
			if (do_command(self, client) < 0) {
				fprintf(stderr, "%s:%d: Could not execute command\n",
					__FUNCTION__, __LINE__);
				/* TODO: Handle error */
			}
			complete -= 1;
		}
		cur = next;
	}
	return (ret);
}
