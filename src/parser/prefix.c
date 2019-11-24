/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prefix.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oyagci <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/01 11:03:45 by oyagci            #+#    #+#             */
/*   Updated: 2019/09/09 14:56:25 by oyagci           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"
#include <stdlib.h>
#include "libft.h"

int				user(unsigned char const *input, char **buffer)
{
	int	ii;

	ii = 0;
	while ((input[ii] >= 0x01 && input[ii] <= 0x09) ||
			(input[ii] == 0x0b || input[ii] == 0x0c) ||
			(input[ii] >= 0x0e && input[ii] <= 0x1f) ||
			(input[ii] >= 0x21 && input[ii] <= 0x3f) ||
			(input[ii] >= 0x41))
		ii++;
	if (ii < 1)
		return (0);
	*buffer = ft_strndup((char *)input, ii);
	return (ii);
}

int				prefix(struct s_prefix *p, char const *input)
{
	int				ii;

	ii = 0;
	if (input[0] != '.')
		while (input[ii] != ' ' && input[ii] != 0)
			ii++;
	if (ii < (int)sizeof(p->data))
	{
		ft_strncpy(p->data, input, ii);
		p->len = ii;
	}
	if (ii == 0)
		return (-1);
	return (0);
}
