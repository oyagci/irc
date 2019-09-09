/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   irc_nick.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oyagci <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/09/09 14:27:36 by oyagci            #+#    #+#             */
/*   Updated: 2019/09/09 14:27:40 by oyagci           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "server.h"

int	irc_nick(struct s_client *c, char **params, int nparams)
{
	char	*nick;

	if (nparams < 1)
	{
		c->server->queuecode(c->server, c, ERR_NONICKNAMEGIVEN);
		return (ERR_NONICKNAMEGIVEN);
	}
	nick = params[0];
	if (ft_strlen(nick) > NICK_SIZE)
	{
		c->server->queuecode(c->server, c, ERR_ERRONEUSNICKNAME);
		return (ERR_ERRONEUSNICKNAME);
	}
	if (!nickadd(&c->server->nicks, nick))
	{
		c->server->queuecode(c->server, c, ERR_NICKNAMEINUSE);
		return (ERR_NICKNAMEINUSE);
	}
	ft_strncpy(c->nickname, nick, NICK_SIZE);
	return (0);
}
