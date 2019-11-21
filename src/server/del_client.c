/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   del_client.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oyagci <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/09/09 14:27:36 by oyagci            #+#    #+#             */
/*   Updated: 2019/09/09 14:27:40 by oyagci           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "server.h"
#include <stdlib.h>
#include <unistd.h>

void	client_delete(struct s_client *self)
{
	close(self->fd);
	self->fd = 0;
	cbuf_free(self->cbuf);
	free(self->username);
	free(self->realname);
}

void	del_client(struct s_server *self, t_client *c)
{
	(void)self;
	client_delete(c);
}
