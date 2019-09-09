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
#include "logger.h"

int	irc_user(struct s_client *c, char **params, int nparams)
{
	if (!c->nickname[0])
		return (0);
	if (nparams < 4)
	{
		c->server->queuecode(c->server, c, ERR_NEEDMOREPARAM);
		return (ERR_NEEDMOREPARAM);
	}
	if (c->is_registered)
		return (ERR_ALREADYREGISTRED);
	c->username = ft_strdup(params[0]);
	set_usermode(c, ft_atoi(params[1]));
	set_realname(c, params[3]);
	c->server->queuecode(c->server, c, RPL_WELCOME);
	c->is_registered = 1;
	return (0);
}
