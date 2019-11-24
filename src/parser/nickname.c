/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   nickname.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oyagci <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/09/09 14:56:20 by oyagci            #+#    #+#             */
/*   Updated: 2019/09/09 14:56:25 by oyagci           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

int			is_valid_char(char c)
{
	return (letter(c) || special(c) || digit(c) || c == '-');
}

int			nickname(char const *input, char **buf)
{
	int	ii;

	ii = 0;
	if (letter(*input) || special(*input))
	{
		ii += 1;
		while (input[ii] && is_valid_char(input[ii]))
			ii += 1;
	}
	if (ii < 9 && ii > 0)
	{
		*buf = ft_strndup(input, ii);
		return (ii);
	}
	return (0);
}
