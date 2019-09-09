/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   err_nickinuse.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oyagci <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/09/09 14:29:03 by oyagci            #+#    #+#             */
/*   Updated: 2019/09/09 14:29:05 by oyagci           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stddef.h>
#include <server.h>
#include <libft.h>

int		err_nickinuse(char *buf, size_t buflen, struct s_client const *c)
{
	(void)c;
	ft_strlcat(buf, ":irc.42.fr ", buflen);
	ft_strlcat(buf, SERR_NICKNAMEINUSE, buflen);
	ft_strlcat(buf, " ", buflen);
	ft_strlcat(buf, "Nickname is in use", buflen);
	return (0);
}
