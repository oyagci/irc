/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oyagci <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/01 10:08:24 by oyagci            #+#    #+#             */
/*   Updated: 2019/11/22 13:44:54 by oyagci           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <signal.h>
#include "server.h"

int	setnosigpipe(void)
{
	signal(SIGPIPE, SIG_IGN);
	return (0);
}

int	main(int ac, char *av[])
{
	int			ret;
	t_server	server;

	ret = 0;
	if (ac < 2)
	{
		ft_putendl_fd("Usage: ./server <port>", 2);
		return (1);
	}
	ret = server_init(&server, ft_atoi(av[1]));
	setnosigpipe();
	if (!ret)
		ret = run(&server);
	return (ret);
}
