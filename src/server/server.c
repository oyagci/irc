/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oyagci <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/01 10:08:24 by oyagci            #+#    #+#             */
/*   Updated: 2019/09/09 14:27:41 by oyagci           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <stdlib.h>
#include "server.h"

int main(int ac, char *av[])
{
	int ret = 0;
	struct s_server server;

	if (ac < 2) {
		printf("Usage: %s <port>\n", av[0]);
		exit(EXIT_FAILURE);
	}
	ret = server_init(&server, ft_atoi(av[1]));
	if (!ret) {
		ret = server.run(&server);
	}
	return (ret);
}
