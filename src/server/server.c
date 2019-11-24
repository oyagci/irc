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

#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include "server.h"
#include "log.h"

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
		fprintf(stderr, "Usage: %s <port>\n", av[0]);
		exit(EXIT_FAILURE);
	}
	ret = server_init(&server, ft_atoi(av[1]));
	setnosigpipe();
	if (!ret)
		ret = run(&server);
	return (ret);
}
