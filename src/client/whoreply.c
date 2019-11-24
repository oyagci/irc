/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   whoreply.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oyagci <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/09/16 11:19:42 by oyagci            #+#    #+#             */
/*   Updated: 2019/09/16 11:19:42 by oyagci           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "client.h"
#include <stdio.h>

int	whoreply(struct s_client *const self,
	struct s_message const *const msg)
{
	size_t i;

	(void)self;
	i = 0;
	while (i < msg->params.nparam)
	{
		ft_putstr((char*)msg->params.param[i]);
		ft_putchar(' ');
		i += 1;
	}
	ft_putchar('\n');
	return (0);
}
