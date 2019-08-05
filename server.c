/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oyagci <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/01 10:08:24 by oyagci            #+#    #+#             */
/*   Updated: 2019/08/05 16:45:24 by oyagci           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <string.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <arpa/inet.h>

#include "irc.h"

int	read_client_command(int cfd, struct s_client_buffer *buffer)
{
	int	ret;

	/*
	** RFC2812 limits the maximum length of a command sent by
	** the client to 512 bytes at most.
	*/
	if ((ret = read(cfd, buffer->data + buffer->len,
					COMMAND_LENGTH - buffer->len)) > 0)
	{
		buffer->len += ret;
		if (strstr(buffer->data, CRLF)) {
			buffer->is_complete = 1;
		}
	}
	return (ret);
}

int	irc_pass(char **params)
{
	(void)params;
	return (1);
}

int	execute_command(char *data)
{
	const struct s_irc_cmds	cmds[] = {
		{ .name = "PASS", .f = irc_pass }
	};
	size_t					ii;
	struct s_message		*msg;

	msg = message(data);
	// Debug Log
	if (msg)
	{
		printf("received command: ");
		if (msg->prefix)
			printf("%s ", msg->prefix->data);
		if (msg->cmd)
			printf("%s ", msg->cmd->data);
		if (msg->params)
		{
			int i = 0;
			while (i < 14 && msg->params->param[i] != 0)
			{
				printf("%s ", msg->params->param[i]);
				i++;
			}
		}
		printf("\n");
	}

	ii = 0;
	while (ii < sizeof(cmds) / sizeof(struct s_irc_cmds))
	{
		if (ft_strequ(cmds[ii].name, msg->cmd->data))
			cmds[ii].f(msg->params->param);
		ii++;
	}

	return (0);
}

int	main(int ac, char *av[])
{
	t_list				*clients;
	struct sockaddr_in	serv_addr;
	int					max_sd;
	struct s_server		server;

	clients = NULL;
	if (ac < 2) {
		printf("Usage: %s <port>\n", av[0]);
		exit(EXIT_FAILURE);
	}

	printf("Creating server on port %s\n", av[1]);

	server.sockfd = socket(AF_INET, SOCK_STREAM, 0);

	memset(&serv_addr, 0, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = INADDR_ANY;
	serv_addr.sin_port = htons(atoi(av[1]));

	bind(server.sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr));
	listen(server.sockfd, MAX_CONN);

	while (42) {
		max_sd = set_fds(server.sockfd, clients, &server.readfds, &server.writefds);
		select(max_sd + 1, &server.readfds, &server.writefds, NULL, NULL);
		handle_new_clients(server.sockfd, &clients, &server.readfds);
		handle_io_clients(&server, clients);
	}
	return (0);
}
