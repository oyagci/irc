/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   event.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oyagci <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/09/09 13:04:48 by oyagci            #+#    #+#             */
/*   Updated: 2019/09/09 13:04:52 by oyagci           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "client.h"

int		client_event(struct s_client *const self, char const *const data)
{
	struct s_event_list const	events[] = {
		{ .s = "JOIN", .f = self->eventjoin },
		{ .s = "PRIVMSG", .f = self->eventprivmsg },
		{ .s = "PART", .f = self->eventpart },
		{ .s = "PING", .f = self->eventping },
		{ .s = "001", .f = self->rpl_welcome },
		{ .s = "372", .f = self->eventmotd },
		{ .s = "375", .f = self->eventmotd },
	};
	size_t						ii;
	struct s_message			msg;

	ft_memset(&msg, 0, sizeof(msg));
	if (message(&msg, data) < 0)
		return (0);
	ii = 0;
	while (ii < sizeof(events) / sizeof(*events))
	{
		if (ft_strequ(events[ii].s, msg.cmd.data))
			events[ii].f(self, &msg);
		ii += 1;
	}
	message_del(&msg);
	return (0);
}
