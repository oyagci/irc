/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server_loop.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oyagci <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/09/09 14:27:37 by oyagci            #+#    #+#             */
/*   Updated: 2019/09/09 14:27:41 by oyagci           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "server.h"

int	run(struct s_server *self)
{
	int	max_sd;

	self->is_running = 1;
	while (self->is_running)
	{
		max_sd = self->set_fds(self);
		select(max_sd + 1, &self->readfds, &self->writefds, NULL, NULL);
		self->accept(self);
		self->read(self);
		self->send(self);
	}
	return (0);
}
