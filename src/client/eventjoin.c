/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   eventjoin.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oyagci <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/09/09 13:04:48 by oyagci            #+#    #+#             */
/*   Updated: 2019/09/09 13:04:52 by oyagci           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "client.h"
#include <stdlib.h>
#include <stdio.h>

int		leave_current_channel(t_client *client)
{
	t_client_msg msg;

	if (!client->channel)
		return (0);
	ft_memset(&msg, 0, sizeof(msg));
	msg.cmd = CMD_LEAVE;
	ft_strncpy(msg.params[0], client->channel->name, SPARAM - 1);
	client_leave(client, &msg);
	return (0);
}

int		eventjoin(struct s_client *const self, struct s_message const *const m)
{
	char				*nick;
	int					ret;
	char const *const	chan = m->params.param[0];

	nick = NULL;
	ret = nickname(m->prefix.data, &nick);
	if (nick)
	{
		if (m->params.nparam >= 1)
		{
			self->channels.addnick(&self->channels, nick, chan);
			if (ft_strnequ(self->nickname, nick, 9))
			{
				leave_current_channel(self);
				self->channel = self->channels.get(&self->channels,
					m->params.param[0]);
			}
			ft_putstr(" * ");
			ft_putstr(nick);
			ft_putstr(" joined channel ");
			ft_putendl((char*)chan);
		}
		free(nick);
	}
	return (ret);
}
