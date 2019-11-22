/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_client.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oyagci <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/09/09 14:27:36 by oyagci            #+#    #+#             */
/*   Updated: 2019/11/22 13:18:39 by oyagci           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "server.h"

t_client	*get_client(struct s_server *self, char const *const nickname)
{
	t_client	*c;
	size_t		i;

	i = 0;
	while (i < NCLIENTS)
	{
		c = self->clients + i;
		if (c->fd > 0 && ft_strnequ(c->nickname, nickname, NICK_SIZE))
			return (c);
		i += 1;
	}
	return (NULL);
}
