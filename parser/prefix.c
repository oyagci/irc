/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prefix.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oyagci <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/01 11:03:45 by oyagci            #+#    #+#             */
/*   Updated: 2019/08/09 14:36:47 by oyagci           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../server.h"
#include <stdio.h>
#include <stdlib.h>
#include "libft.h"

// char				*shortname(char const *input)
// {
// 	char	*sn;
// 	int		ii;
// 
// 	ii = 0;
// 	if (letter(*input) || digit(*input))
// 	{
// 		while (letter(input[ii]) || digit(input[ii]) || input[ii] == '-')
// 			ii++;
// 	}
// 	return (ft_strndup((char *)input, ii));
// }

// char				*hostname(char const *input)
// {
// 	char	*hn;
// 	char	*s;
// 
// 	hn = ft_memalloc(512);
// 	s = shortname(input);
// 	if (s && ft_strlen(s) > 0)
// 	{
// 		input += ft_strlen(s);
// 		strcat(hn, s);
// 		while (*input == '.')
// 		{
// 			input++;
// 			s = shortname(input);
// 			input += ft_strlen(s);
// 			strcat(hn, ".");
// 			strcat(hn, s);
// 		}
// 	}
// 	else
// 	{
// 		free(hn);
// 		hn = NULL;
// 	}
// 	return (hn);
// }

// int		servername(char const *input, struct s_servername **s)
// {
// 	int					len;
// 
// 	len = 0;
// 	*s = ft_memalloc(sizeof(*s));
// 	if (s)
// 	{
// 		(*s)->hostname = hostname(input);
// 		if ((*s)->hostname)
// 			len = ft_strlen((*s)->hostname);
// 		else
// 			servername_del(s);
// 	}
// 	return (len);
// }

int				user(unsigned char const *input, char **buffer)
{
	int	ii;

	ii = 0;
	while ((input[ii] >= 0x01 && input[ii] <= 0x09) ||
			(input[ii] == 0x0b || input[ii] == 0x0c) ||
			(input[ii] >= 0x0e && input[ii] <= 0x1f) ||
		  	(input[ii] >= 0x21 && input[ii] <= 0x3f) ||
			(input[ii] >= 0x41))
		ii++;
	if (ii < 1)
		return (0);
	*buffer = ft_strndup((char *)input, ii);
	return (ii);
}

struct s_prefix	*prefix(char const *input)
{
	struct s_prefix	*p;
	int				ii;

	p = ft_memalloc(sizeof(*p));
	if (!p)
		return (NULL);
	
	ii = 0;
	if (input[0] != '.')
		while (input[ii] != ' ' && input[ii] != 0)
		ii++;
	p->data = ft_strndup((char *)input, ii);
	p->len = ii;
	if (!p->data)
	{
		free(p);
		return (NULL);
	}
	return (p);
}

void			prefix_del(struct s_prefix **p)
{
	if (*p)
	{
		free((*p)->data);
		free(*p);
		*p = NULL;
	}
}
