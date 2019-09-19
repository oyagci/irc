/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oyagci <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/09/09 13:04:49 by oyagci            #+#    #+#             */
/*   Updated: 2019/09/09 13:04:52 by oyagci           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "client.h"

int		quit(struct s_client *const self,
	struct s_client_msg const *const clientmsg)
{
	char	*msg;

	msg = ft_strnew(512);
	if (!msg)
		return (-1);
	ft_strlcat(msg, "QUIT :", 513);
	if (clientmsg->nparam > 0)
	{
		ft_strlcat(msg, clientmsg->params[clientmsg->nparam - 1], 513);
	}
	ft_strlcat(msg, CRLF, 513);
	self->queuemsg(self, msg);
	return (0);
}
