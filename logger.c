/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   logger.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oyagci <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/01 13:18:53 by oyagci            #+#    #+#             */
/*   Updated: 2019/08/06 15:55:37 by oyagci           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "logger.h"
#include <stdio.h>

void	printprefix(int level)
{
	char	*prefix;

	if (level == 0)
		prefix = PREFIX_DEBUG;
	else if (level == 1)
		prefix = PREFIX_WARN;
	else if (level == 3)
		prefix = PREFIX_ERR;
	else
		prefix = PREFIX_DEFAULT;
	printf("%s ", prefix);
}

void	msglog(int level, char *s)
{
	char	*prefix;

	if (level >= LOGLEVEL)
	{
		if (level == 0)
			prefix = PREFIX_DEBUG;
		else if (level == 1)
			prefix = PREFIX_WARN;
		else if (level == 3)
			prefix = PREFIX_ERR;
		else
			prefix = PREFIX_DEFAULT;
		printf("%s %s\n", prefix, s);
	}
}
