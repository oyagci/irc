/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   irc_part.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oyagci <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/09/09 14:27:36 by oyagci            #+#    #+#             */
/*   Updated: 2019/09/09 14:27:40 by oyagci           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "server.h"

int				irc_part(struct s_client *client, struct s_params *p)
{
	struct s_server		*s;
	struct s_channel	*chan;
	int					ret;

	if (!client->is_registered)
		return (0);
	s = client->server;
	if (p->nparam <= 0)
	{
		queue_code_reply(s, client, ERR_NEEDMOREPARAM);
		return (0);
	}
	chan = get_channel(s, p->param[0]);
	if (chan)
	{
		ret = rm_from_chan(client->nickname, chan);
		if (ret == ERR_NOTONCHANNEL)
			queue_code_reply(s, client, ERR_NOSUCHCHANNEL);
		else
			notifypart(s, chan, client->nickname);
	}
	else
		queue_code_reply(s, client, ERR_NOSUCHCHANNEL);
	return (0);
}
