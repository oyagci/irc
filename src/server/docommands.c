/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   docommands.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oyagci <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/22 13:56:59 by oyagci            #+#    #+#             */
/*   Updated: 2019/11/22 13:57:10 by oyagci           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "server.h"

static int	docommand(struct s_server *self, struct s_client *client)
{
	char			buf[512];
	unsigned char	data;
	int				ii;

	data = 0;
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
	return (execute_command(self, client, buf));
}

int			docommands(struct s_server *self)
{
	t_client	*c;
	size_t		i;

	i = 0;
	while (i < NCLIENTS)
	{
		c = self->clients + i;
		if (c->fd > 0)
		{
			while (c->ncmds > 0)
			{
				docommand(self, c);
				c->ncmds -= 1;
			}
		}
		i += 1;
	}
	return (0);
}
