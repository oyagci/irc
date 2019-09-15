/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oyagci <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/01 11:21:16 by oyagci            #+#    #+#             */
/*   Updated: 2019/09/09 14:56:25 by oyagci           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"
#include <libft.h>
#include <stdlib.h>
#include <stdio.h>

static int				set_cmd(struct s_command *buf, char const *input)
{
	int	i;

	i = 0;
	while (ft_isalpha(input[i]))
		i += 1;
	if (i == 0 || i >= SCMD)
		return (-1);
	ft_strncpy(buf->data, input, i);
	buf->len = ft_strlen(buf->data);
	return (0);
}

static int				set_num(struct s_command *buf, char const *input)
{
	int	i;

	i = 0;
	while (ft_isdigit(input[i]))
	{
		if (i >= 3)
			return (-1);
		i += 1;
	}
	if (i >= SCMD)
		return (-1);
	ft_strncpy(buf->data, input, i);
	buf->len = ft_strlen(buf->data);
	return (0);
}

int						command(struct s_command *const cmd, char const *input)
{
	if (ft_isalpha(*input))
		return (set_cmd(cmd, input));
	else if (ft_isdigit(*input))
		return (set_num(cmd, input));
	else
		return (-1);
	return (0);
}
