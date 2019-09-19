/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   message.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oyagci <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/01 10:08:21 by oyagci            #+#    #+#             */
/*   Updated: 2019/09/09 14:56:25 by oyagci           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"
#include <stdlib.h>

int					message(struct s_message *msg, char const *input)
{
	if (input[0] == ':')
	{
		input += 1;
		if (prefix(&msg->prefix, input) < 0 || input[msg->prefix.len] != ' ')
			return (-1);
		input += msg->prefix.len + 1;
	}
	if (!command(&msg->cmd, input))
	{
		input += msg->cmd.len;
		params(&msg->params, input);
		input += msg->params.len;
		return (crlf(input));
	}
	return (0);
}
