/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   irc.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oyagci <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */ /*   Created: 2019/08/01 10:09:05 by oyagci            #+#    #+#             */
/*   Updated: 2019/08/01 15:15:19 by oyagci           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef IRC_H
# define IRC_H

#include <string.h>
#include "libft.h"

struct s_message
{
	struct s_prefix		*prefix;
	struct s_command	*cmd;
	struct s_params		*params;
	size_t				len;
};

struct s_prefix
{
	size_t	len;
};

struct s_command
{
	char	*data;
	size_t	len;
};

struct s_crlf
{
};

/*
** RFC2812 tells us that the maximum number of parameters
** per message is 15 (fifteen)
*/
struct s_params
{
	char	*param[15];
	size_t	len;
};

struct s_message	*message(char const *input);
struct s_prefix		*prefix(char const *input);
struct s_command	*command(char const *input);
struct s_crlf		*crlf(char const *input);
struct s_params		*params(char const *input);

void				prefix_del(struct s_prefix **p);
void				command_del(struct s_command **cmd);
void				message_del(struct s_message **msg);

#endif
