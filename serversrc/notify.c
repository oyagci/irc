/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   notify.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oyagci <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/09/09 14:27:36 by oyagci            #+#    #+#             */
/*   Updated: 2019/09/09 14:27:40 by oyagci           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "server.h"

int			notifypart(struct s_server *s, struct s_channel *chan,
	char const *const nick)
{
	t_list			*elem;
	struct s_client	*c;
	char			notif[513];

	ft_memset(notif, 0, 513);
	elem = chan->clients;
	ft_strlcat(notif, ":", 513);
	ft_strlcat(notif, nick, 513);
	ft_strlcat(notif, " ", 513);
	ft_strlcat(notif, "PART", 513);
	ft_strlcat(notif, " ", 513);
	ft_strlcat(notif, chan->name, 513);
	ft_strlcat(notif, CRLF, 513);
	while (elem)
	{
		c = elem->content;
		s->queuenotif(s, c, notif);
		elem = elem->next;
	}
	return (0);
}
