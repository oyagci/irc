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

t_client	*get_client(struct s_server *self, char const *const nickname)
{
	t_client	*c;
	size_t		i;

	i = 0;
	while (i < self->nclients)
	{
		c = self->clients + i;
		if (ft_strnequ(c->nickname, nickname, NICK_SIZE))
			return (c);
		i += 1;
	}
	return (NULL);
}
