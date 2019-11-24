/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   nick.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oyagci <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/09/09 14:27:36 by oyagci            #+#    #+#             */
/*   Updated: 2019/09/09 14:27:40 by oyagci           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "nick.h"
#include "libft.h"
#include <stdlib.h>
#include "server.h"

int		nickavail(t_nicktable *nicks, char const *nick)
{
	size_t	i;

	i = 0;
	while (i < 10)
	{
		if (ft_strequ(nicks->table[i], nick))
			return (0);
		i++;
	}
	return (1);
}

static int	get_empty_nick(t_nicktable *nicks)
{
	int	id;

	id = 0;
	while (id < 10)
	{
		if (nicks->table[id][0] == 0)
			return (id);
		id += 1;
	}
	return (-1);
}

int		nickadd(t_nicktable *nicks, char const *nick)
{
	int	id;

	if (!nickavail(nicks, nick))
		return (0);
	id = get_empty_nick(nicks);
	if (id < 0)
		return (0);
	ft_strlcat(nicks->table[id], nick, 10);
	return (1);
}

int		nickinit(t_nicktable *nicks)
{
	ft_memset(nicks, 0, sizeof(*nicks));
	return (0);
}

void	nickremove(t_nicktable *nicks, char *name)
{
	size_t	i;

	i = 0;
	while (i < 10)
	{
		if (ft_strnequ(name, nicks->table[i], NICK_SIZE))
		{
			ft_memset(nicks->table[i], 0, 10);
			break ;
		}
		i += 1;
	}
}
