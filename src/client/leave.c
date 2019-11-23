/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   leave.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oyagci <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/09/09 13:04:49 by oyagci            #+#    #+#             */
/*   Updated: 2019/09/09 13:04:52 by oyagci           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "client.h"
#include <stdio.h>

int		client_leave(struct s_client *const self,
	struct s_client_msg const *const cmd)
{
	char	*msg;

	if (!self->servsock)
	{
		printf(" - Not connected to a server\n");
		return (0);
	}
	msg = ft_strnew(512);
	if (!msg)
		return (-1);
	ft_strlcat(msg, "PART", 512);
	ft_strlcat(msg, " ", 512);
	ft_strlcat(msg, cmd->params[0], 512);
	ft_strlcat(msg, CRLF, 512);
	self->channel = 0;
	printf(" - Leaving channel %s\n", cmd->params[0]);
	return (self->queuemsg(self, msg));
}
