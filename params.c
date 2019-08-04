/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   params.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oyagci <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/01 15:53:41 by oyagci            #+#    #+#             */
/*   Updated: 2019/08/01 16:42:26 by oyagci           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "irc.h"
#include "libft.h"
#include <stdio.h>
#include "logger.h"

int					is_nospcrlfcl(unsigned char c)
{
	return ((c >= 0x01 && c <= 0x09) ||
			(c >= 0x0b && c <= 0x0c) ||
			(c >= 0x0e && c <= 0x1f) ||
			(c >= 0x21 && c <= 0x39) ||
			(c >= 0x3b /* &&  c <= 0xff */));
}

char				*extract_param(char const *input)
{
	int	i;

	i = 0;
	if (is_nospcrlfcl(input[i]))
	{
		i += 1;
		while (is_nospcrlfcl(input[i]) || input[i] == ':')
			i += 1;
	}
	printf("Param: %.*s\n", i, input);
	return (ft_strndup((char *)input, i));
}

char				*extract_trailing(char const *input)
{
	int	i;

	i = 0;
	while (is_nospcrlfcl(input[i]) || input[i] == ':' || input[i] == ' ')
		i++;
<<<<<<< HEAD
	printf("Last param: %.*s (%d)\n", i, input, i);
=======
	printf("Last param: %.*s\n", i, input);
>>>>>>> 7c0d32c25107e5c52af9b99ee03376c37b353098
	return (ft_strndup((char *)input, i));
}

/*
** params     =  *14( SPACE middle ) [ SPACE ":" trailing ]
**		      =/ 14( SPACE middle ) [ SPACE [ ":" ] trailing ]
** middle     =  nospcrlfcl *( ":" / nospcrlfcl )
** nospcrlfcl =  %x01-09 / %x0B-0C / %x0E-1F / %x21-39 / %x3B-FF
**				; any octet except NUL, CR, LF, " " and ":"
** trailing   =  *( ":" / " " / nospcrlfcl )
*/
struct s_params		*params(char const *input)
{
	struct s_params *p;
	int	i;
	int	j;

	msglog(LOGDEBUG, "Parsing params");
	p = ft_memalloc(sizeof(struct s_params));
	j = 0;
	i = 0;
	if (p)
	{
		while (j < 14 && input[i] == ' ' && input[i + 1] != ':')
		{
			i += 1;
			p->param[j] = extract_param(input + i);
			i += ft_strlen(p->param[j]);
			j += 1;
		}
		if (input[i] == ' ')
		{
			i += 1;
			if (input[i] == ':')
			{
				i += 1;
				p->param[j] = extract_trailing(input + i);
				i += ft_strlen(p->param[j]);
			}
			else if (j == 14)
			{
				msglog(LOGDEBUG, "Parsing trailing");
				p->param[j] = extract_trailing(input + i);
				i += ft_strlen(p->param[j]);
			}
		}
	}
	p->len = i;
	return (p);
}
