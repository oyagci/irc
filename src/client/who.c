/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   who.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oyagci <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/09/09 13:04:49 by oyagci            #+#    #+#             */
/*   Updated: 2019/09/09 13:04:53 by oyagci           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "client.h"
#include <stdio.h>
#include <stdlib.h>

int		client_who(struct s_client *const self,
	struct s_client_msg const *const cmd)
{
	char	*msg;

	(void)cmd;
	if (!self->channel)
	{
		ft_putendl(" * Not in a channel");
		return (0);
	}
	if (!self->servsock)
	{
		ft_putendl(" * Not connected to a server");
		return (0);
	}
	msg = ft_strnew(513);
	if (!msg)
		return (-1);
	ft_strlcat(msg, "WHO ", 513);
	ft_strlcat(msg, self->channel->name, 513);
	ft_strlcat(msg, CRLF, 513);
	self->queuemsg(self, msg);
	return (0);
}
