/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   irc_user.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oyagci <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/09/09 14:27:36 by oyagci            #+#    #+#             */
/*   Updated: 2019/11/22 12:48:57 by oyagci           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "server.h"

int	irc_user(struct s_client *c, struct s_params *p)
{
	if (!c->nickname[0])
		return (0);
	if (p->nparam < 4)
	{
		queue_code_reply(c->server, c, ERR_NEEDMOREPARAM);
		return (ERR_NEEDMOREPARAM);
	}
	if (c->is_registered)
		return (ERR_ALREADYREGISTRED);
	queue_code_reply(c->server, c, RPL_WELCOME);
	c->is_registered = 1;
	return (0);
}
