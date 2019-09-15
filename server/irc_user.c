/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   irc_user.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oyagci <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/09/09 14:27:36 by oyagci            #+#    #+#             */
/*   Updated: 2019/09/09 14:27:40 by oyagci           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "server.h"

int	irc_user(struct s_client *c, struct s_params *p)
{
	if (!c->nickname[0])
		return (0);
	if (p->nparam < 4)
	{
		c->server->queuecode(c->server, c, ERR_NEEDMOREPARAM);
		return (ERR_NEEDMOREPARAM);
	}
	if (c->is_registered)
		return (ERR_ALREADYREGISTRED);
	c->username = ft_strdup(p->param[0]);
	set_usermode(c, ft_atoi(p->param[1]));
	set_realname(c, p->param[3]);
	c->server->queuecode(c->server, c, RPL_WELCOME);
	c->is_registered = 1;
	return (0);
}
