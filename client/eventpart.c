/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   eventpart.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oyagci <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/09/09 13:04:49 by oyagci            #+#    #+#             */
/*   Updated: 2019/09/09 13:04:52 by oyagci           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "client.h"
#include <stdio.h>
#include <stdlib.h>

int	eventpart(struct s_client *const self, struct s_message const *const cmd)
{
	char		*nick;
	char const	*chan;

	nickname(cmd->prefix.data, &nick);
	chan = cmd->params.param[0];
	if (nick && chan)
	{
		self->channels.rmnick(&self->channels, nick, chan);
		printf(" * %s has left %s\n", nick, chan);
		free(nick);
	}
	return (0);
}
