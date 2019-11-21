/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   irc_quit.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oyagci <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/09/09 14:27:36 by oyagci            #+#    #+#             */
/*   Updated: 2019/09/09 14:27:40 by oyagci           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "server.h"
#include <libft.h>
#include <unistd.h>

int		irc_quit(struct s_client *c, struct s_params *p)
{
	(void)p;
	c->should_be_disconnected = 1;
	return (0);
}
