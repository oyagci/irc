/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   set_usermode.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oyagci <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/09/09 14:27:37 by oyagci            #+#    #+#             */
/*   Updated: 2019/09/09 14:27:41 by oyagci           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "server.h"

int		set_usermode(struct s_client *c, int mode)
{
	mode &= 0xc;
	if (mode & 0x8)
		c->umode &= UMODE_INVISIBLE;
	if (mode & 0x4)
		c->umode &= UMODE_WALLOPS;
	return (0);
}

int		set_realname(struct s_client *c, char *rn)
{
	c->realname = ft_strdup(rn);
	return (0);
}
