/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oyagci <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/01 11:21:16 by oyagci            #+#    #+#             */
/*   Updated: 2019/08/09 11:54:14 by oyagci           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../irc.h"
#include <libft.h>
#include <stdlib.h>
#include <stdio.h>

/*
** command =  1*letter / 3digit
*/
struct s_command	*command(char const *input)
{
	struct s_command	*cmd;
	int					i;

	cmd = ft_memalloc(sizeof(*cmd));
	if (!cmd)
		return (NULL);
	i = 0;
	if (ft_isalpha(input[0]))
	{
		while (ft_isalpha(input[i]))
			i += 1;
		cmd->data = ft_strndup((char *)input, i);
		cmd->len = ft_strlen(cmd->data);
	}
	else if (ft_isdigit(input[0]))
	{
		while (ft_isdigit(input[0]))
		{
			i += 1;
			if (i >= 3)
			{
				command_del(&cmd);
				return (NULL);
			}
		}
		cmd->data = ft_strndup((char *)input, i);
		cmd->len = ft_strlen(cmd->data);
	}
	else
	{
		free(cmd);
		cmd = NULL;
	}
	return (cmd);
}

void			command_del(struct s_command **cmd)
{
	if (*cmd)
	{
		free((*cmd)->data);
		free(*cmd);
		*cmd = NULL;
	}
}
