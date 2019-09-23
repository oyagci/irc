/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   channel.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oyagci <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/09/09 14:27:36 by oyagci            #+#    #+#             */
/*   Updated: 2019/09/09 14:27:40 by oyagci           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "server.h"
#include <stdlib.h>

static int client_in_channel(struct s_client const *client,
	struct s_channel const *channel)
{
	int ret = 0;

	for (t_list *elem = channel->clients; elem != NULL; elem = elem->next) {
		struct s_client *cur = elem->content;

		if (ft_strnequ(client->nickname, cur->nickname, NICK_SIZE)) {
			ret = 1;
			break ;
		}
	}
	return (ret);
}

int channel_add_client(struct s_channel *channel, struct s_client *client)
{
	t_list *elem = NULL;
	int ret = 1;

	if (!client_in_channel(client, channel)) {
		elem = ft_lstnew(0, 0);
		if (elem) {
			elem->content = client;
			ft_lstpush(&channel->clients, elem);
			ret = 0;
		}
		else {
			ret = -1;
		}
	}
	return (ret);
}
