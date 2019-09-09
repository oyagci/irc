/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   irc_quit.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oyagci <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/09/09 14:27:36 by oyagci            #+#    #+#             */
/*   Updated: 2019/09/09 14:27:40 by oyagci           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "server.h"
#include <libft.h>
#include <unistd.h>
#include "logger.h"

void	remove_from_all_channels(struct s_client *const c)
{
	t_list				*l;
	struct s_channel	*chan;

	l = c->server->channels;
	while (l != NULL)
	{
		chan = l->content;
		rm_from_chan(c->nickname, chan);
		c->server->notifypart(c->server, chan, c->nickname);
		l = l->next;
	}
}

int		irc_quit(struct s_client *c, char **params, int nparams)
{
	LOG(LOGDEBUG, "QUIT");
	(void)params;
	(void)nparams;
	remove_from_all_channels(c);
	nickremove(&c->server->nicks, c->nickname);
	c->server->del_client(c->server, c);
	return (0);
}
