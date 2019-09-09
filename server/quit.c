/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oyagci <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/09/09 14:27:36 by oyagci            #+#    #+#             */
/*   Updated: 2019/09/09 14:27:40 by oyagci           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "server.h"
#include "logger.h"
#include <stdlib.h>
#include <unistd.h>

int			quit(struct s_server *self, struct s_client *client,
	char const *const msg)
{
	char	*notif;

	notif = ft_strnew(512);
	ft_strlcat(notif, "QUIT", 512);
	ft_strlcat(notif, " :", 512);
	ft_strlcat(notif, msg, 512);
	self->queuenotif(self, client, notif);
	return (0);
}
