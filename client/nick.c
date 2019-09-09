/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   nick.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oyagci <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/09/09 13:04:49 by oyagci            #+#    #+#             */
/*   Updated: 2019/09/09 13:04:52 by oyagci           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "client.h"

int		client_nick(struct s_client *const self,
					struct s_client_msg const *const cmd)
{
	char	*msg;
	int		ret;

	ret = 0;
	ft_strncpy(self->nickname, cmd->params[0], SNICK);
	if (self->servsock > 0)
	{
		msg = ft_strnew(513);
		if (msg)
		{
			ft_strlcat(msg, "NICK", 513);
			ft_strlcat(msg, " ", 513);
			ft_strlcat(msg, cmd->params[0], 513);
			ft_strlcat(msg, CRLF, 513);
			self->queuemsg(self, msg);
		}
		else
			ret = -1;
	}
	return (ret);
}
