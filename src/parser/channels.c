/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   channels.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oyagci <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/09/09 14:56:20 by oyagci            #+#    #+#             */
/*   Updated: 2019/09/09 14:56:25 by oyagci           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft/includes/libft.h"
#include "parser.h"
#include <stdlib.h>

t_list		*channels(char *input)
{
	t_list	*chans;
	t_list	*elem;
	int		ret;
	char	*chanbuf;

	chans = NULL;
	while ((ret = chanstr(input, &chanbuf)) > 0)
	{
		elem = ft_lstnew(0, 0);
		elem->content = chanbuf;
		ft_lstpush(&chans, elem);
		input += ret;
		if (*input == ',')
			input++;
	}
	return (chans);
}

void		channels_delone(void *c, size_t s)
{
	(void)s;
	free(c);
}

void		channels_del(t_list **chansp)
{
	ft_lstdel(chansp, &channels_delone);
}
