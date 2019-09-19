/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rpl_welcome.c                                      :+:      :+:    :+:   */
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

int		rpl_welcome(char *buf, size_t buflen, struct s_client const *c)
{
	ft_strlcpy(buf, ":irc.42.fr ", buflen);
	ft_strlcat(buf, SRPL_WELCOME, buflen);
	ft_strlcat(buf, " ", buflen);
	ft_strlcat(buf, ":Welcome to the Internet Relay Chat, ", buflen);
	ft_strlcat(buf, c->nickname, buflen);
	ft_strlcat(buf, "!", buflen);
	ft_strlcat(buf, CRLF, buflen);
	return (0);
}
