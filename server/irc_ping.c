/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   irc_ping.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oyagci <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/09/09 14:27:36 by oyagci            #+#    #+#             */
/*   Updated: 2019/09/09 14:27:40 by oyagci           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <time.h>
#include "server.h"

int		irc_ping(struct s_client *client, char **params, int nparam)
{
	char			pingmsg[512];
	struct s_server	*s;

	ft_memset(pingmsg, 0, 512);
	(void)params;
	(void)nparam;
	s = client->server;
	ft_strlcat(pingmsg, "PING", 512);
	ft_strlcat(pingmsg, CRLF, 512);
	s->queuenotif(s, client, pingmsg);
	return (0);
}
