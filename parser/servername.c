/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   servername.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oyagci <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/09/09 14:56:21 by oyagci            #+#    #+#             */
/*   Updated: 2019/09/09 14:56:25 by oyagci           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

struct s_servername	*servername(char *input)
{
	struct s_servername	*s;

	s = ft_memalloc(sizeof(*s));
	if (s)
	{
		s->hostname = hostname(s);
		s->len += s->hostname->len;
	}
	return (s);
}

void				servername_del(struct s_servername **sn)
{
	hostname_del((*sn)->hostname);
	free(*sn);
	*sn = NULL;
}
