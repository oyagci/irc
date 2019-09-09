/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_client.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oyagci <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/09/09 14:27:36 by oyagci            #+#    #+#             */
/*   Updated: 2019/09/09 14:27:40 by oyagci           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "server.h"

struct s_client	*get_client(struct s_server *self, char const *const nickname)
{
	t_list			*clients;
	struct s_client	*c;

	clients = self->clients;
	while (clients)
	{
		c = clients->content;
		if (ft_strnequ(c->nickname, nickname, NICK_SIZE))
			return (c);
		clients = clients->next;
	}
	return (NULL);
}
