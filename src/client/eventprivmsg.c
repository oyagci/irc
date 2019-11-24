/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   eventprivmsg.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oyagci <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/09/09 13:04:49 by oyagci            #+#    #+#             */
/*   Updated: 2019/09/09 13:04:52 by oyagci           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "client.h"
#include <stdio.h>

int		eventprivmsg(struct s_client *const self,
	struct s_message const *const m)
{
	char const	*from;
	char const	*msg;
	char const	*channel;

	(void)self;
	from = m->prefix.data;
	channel = m->params.param[0];
	msg = m->params.param[1];
	ft_putstr("[");
	ft_putstr(BOLDRED);
	ft_putstr((char*)channel);
	ft_putstr("] ");
	ft_putstr(UNDERLINE);
	ft_putstr((char*)from);
	ft_putstr(RESET);
	ft_putstr(": ");
	ft_putendl((char*)msg);
	return (0);
}
