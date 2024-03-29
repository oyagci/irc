/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server_init.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oyagci <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/09/09 14:27:37 by oyagci            #+#    #+#             */
/*   Updated: 2019/09/09 14:27:41 by oyagci           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <stdlib.h>
#include <unistd.h>
#include "server.h"

static int	startup(struct s_server *server, unsigned int port)
{
	struct sockaddr_in	serv_addr;

	server->sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (server->sockfd <= 0)
		return (-1);
	ft_memset(&serv_addr, 0, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	serv_addr.sin_port = htons(port);
	if (-1 == bind(server->sockfd, (struct sockaddr *)&serv_addr,
			sizeof(serv_addr)))
		return (-1);
	else if (-1 == listen(server->sockfd, MAX_CONN))
		return (-1);
	return (0);
}

int			server_init(struct s_server *server, unsigned int port)
{
	ft_memset(server, 0, sizeof(*server));
	nickinit(&server->nicks);
	return (startup(server, port));
}
