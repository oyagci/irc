/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   irc_pong.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oyagci <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/09/09 14:27:36 by oyagci            #+#    #+#             */
/*   Updated: 2019/09/09 14:27:40 by oyagci           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "server.h"
#include <time.h>

int		irc_pong(struct s_client *client, struct s_params *p)
{
	struct timespec	current;

	(void)p;
	clock_gettime(CLOCK_REALTIME, &current);
	client->ping.tv_sec = current.tv_sec + 10;
	client->timeout.tv_sec = current.tv_sec;
	return (0);
}
