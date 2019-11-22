/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   channel.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oyagci <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/09/09 14:27:36 by oyagci            #+#    #+#             */
/*   Updated: 2019/11/22 13:55:18 by oyagci           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "server.h"
#include <stdlib.h>

static int	client_in_channel(struct s_client const *client,
	struct s_channel const *channel)
{
	int			ret;
	t_list		*elem;
	t_client	*cur;

	ret = 0;
	elem = channel->clients;
	while (elem)
	{
		cur = elem->content;
		if (ft_strnequ(client->nickname, cur->nickname, NICK_SIZE))
		{
			ret = 1;
			break ;
		}
		elem = elem->next;
	}
	return (ret);
}

int			channel_add_client(t_channel *channel, t_client *client)
{
	t_list	*elem;
	int		ret;

	ret = 1;
	elem = NULL;
	if (!client_in_channel(client, channel))
	{
		elem = ft_lstnew(0, 0);
		if (elem)
		{
			elem->content = client;
			ft_lstpush(&channel->clients, elem);
			ret = 0;
		}
		else
			ret = -1;
	}
	return (ret);
}
