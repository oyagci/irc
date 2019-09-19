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
	cbuf_free(self->cbuf);
	free(self->username);
	free(self->realname);
	free(self);
}

void	del_client(struct s_server *self, struct s_client *c)
{
	t_list	*elem;
	t_list	*prev;
	t_list	*next;

	next = NULL;
	prev = NULL;
	elem = self->clients;
	while (elem)
	{
		next = elem->next;
		if (elem->content == c)
		{
			client_delete(c);
			if (prev)
				prev->next = elem->next;
			else
				self->clients = elem->next;
			free(elem);
		}
		prev = elem;
		elem = next;
	}
}
