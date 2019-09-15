/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   params.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oyagci <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/01 15:53:41 by oyagci            #+#    #+#             */
/*   Updated: 2019/09/09 14:56:25 by oyagci           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"
#include "libft.h"
#include <stdlib.h>

static void			extract_param(char *buf, char const *input)
{
	int	i;

	i = 0;
	if (is_nospcrlfcl(input[i]))
	{
		i += 1;
		while (input[i] != 0 && (is_nospcrlfcl(input[i]) || input[i] == ':'))
			i += 1;
	}
	if (i < SPARAM)
		ft_strncpy(buf, input, i);
}

static void			extract_trailing(char *buf, char const *input)
{
	int	i;

	i = 0;
	while (is_nospcrlfcl(input[i]) || input[i] == ':' || input[i] == ' ')
		i++;
	if (i < SPARAM)
		ft_strncpy(buf, input, i);
}

int					last_param(struct s_params *p, char const *input, int i,
	int j)
{
	if (input[i] == ' ')
	{
		i += 1;
		if (input[i] == ':' || j == 14)
		{
			input[i] == ':' ? (i++) : 0;
			extract_trailing(p->param[j], input + i);
			p->nparam += 1;
			i += ft_strlen(p->param[j]);
		}
	}
	return (i);
}

int					params(struct s_params *p, char const *input)
{
	int				i;

	i = 0;
	p->nparam = 0;
	while (p->nparam < 14 && input[i] == ' ' && input[i + 1] != ':')
	{
		i += 1;
		ft_memset(p->param[p->nparam], 0, SPARAM);
		extract_param(p->param[p->nparam], input + i);
		i += ft_strlen(p->param[p->nparam]);
		p->nparam += 1;
	}
	i = last_param(p, input, i, p->nparam);
	p->len = i;
	return (0);
}
