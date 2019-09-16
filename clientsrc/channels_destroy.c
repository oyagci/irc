/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   channels_destroy.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oyagci <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/09/16 12:31:25 by oyagci            #+#    #+#             */
/*   Updated: 2019/09/16 12:43:57 by oyagci           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "channels.h"
#include <stdlib.h>

void	delclient(void *clientp, size_t sz)
{
	(void)sz;
	free(clientp);
}

void	delchan(void *chanp, size_t sz)
{
	struct s_chan	*chan;

	(void)sz;
	chan = chanp;
	ft_lstdel(&chan->clients, delclient);
	free(chan);
}

void	channels_destroy(struct s_channels *channels)
{
	ft_lstdel(&channels->list, delchan);
}
