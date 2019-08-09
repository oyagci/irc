/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   crlf.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oyagci <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/01 11:52:49 by oyagci            #+#    #+#             */
/*   Updated: 2019/08/09 11:52:20 by oyagci           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../irc.h"
#include <stdlib.h>

struct s_crlf	*crlf(char const *input)
{
	struct s_crlf	*c;

	c = NULL;
	if (input[0] != '\0' && input[0] == 0x0d && input[1] == 0x0a)
	{
		c = ft_memalloc(sizeof(*c));
	}
	return c;
}

void crlf_del(struct s_crlf **c)
{
	if (*c)
	{
		free(*c);
		*c = NULL;
	}
}
