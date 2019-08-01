/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prefix.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oyagci <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/01 11:03:45 by oyagci            #+#    #+#             */
/*   Updated: 2019/08/01 15:15:42 by oyagci           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "irc.h"
#include <stdlib.h>

struct s_prefix	*prefix(char const *input)
{
	(void)input;
	return (NULL);
}

void			prefix_del(struct s_prefix **p)
{
	if (*p)
	{
		free(*p);
		*p = NULL;
	}
}
