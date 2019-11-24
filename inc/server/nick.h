/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   nick.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oyagci <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/09/09 14:28:08 by oyagci            #+#    #+#             */
/*   Updated: 2019/09/09 14:28:09 by oyagci           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef NICK_H
# define NICK_H

# include <stddef.h>

typedef struct	s_nicktable {
	char	table[10][10];
}				t_nicktable;

int				nickinit(t_nicktable *nicks);
int				nickavail(t_nicktable *nicks, char const *nick);
int				nickadd(t_nicktable *nicks, char const *nick);
void			nickremove(t_nicktable *nicks, char *name);

#endif
