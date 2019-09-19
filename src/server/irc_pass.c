/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   irc_pass.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oyagci <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/09/09 14:27:36 by oyagci            #+#    #+#             */
/*   Updated: 2019/09/09 14:27:40 by oyagci           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "server.h"

int	irc_pass(struct s_client *c, struct s_params *p)
{
	if (c->is_connected)
	{
		c->server->queuecode(c->server, c, ERR_ALREADYREGISTRED);
		return (ERR_ALREADYREGISTRED);
	}
	if (SERVER_PASS)
	{
		if (p->nparam < 1)
		{
			c->server->queuecode(c->server, c, ERR_NEEDMOREPARAM);
			return (ERR_NEEDMOREPARAM);
		}
		if (ft_strcmp(p->param[0], SERVER_PASS))
		{
			c->server->queuecode(c->server, c, ERR_PASSWDMISMATCH);
			return (ERR_PASSWDMISMATCH);
		}
	}
	c->is_connected = 1;
	return (0);
}