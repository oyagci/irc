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
#include <stdio.h>
#include "log.h"

static void init_methods(struct s_server *s)
{
	s->run = &run;
	s->accept = &accept_new_clients;
	s->read = &read_client_command;
	s->send = &send_queued_replies;
	s->docommands = &docommands;
	s->exec_cmd = &execute_command;
	s->queuecode = &queue_code_reply;
	s->queuenotif = &queue_reply;
	s->rm_from_chan = &rm_from_chan;
	s->get_channel = &get_channel;
	s->notifypart = &notifypart;
	s->create_channel = &create_channel;
	s->add_to_chan = &add_to_chan;
	s->pinginactive = &pinginactive;
	s->quit = &quit;
	s->get_client = &get_client;
	s->del_client = &del_client;
	s->update_clients = update_clients;
}

static int startup(struct s_server *server, unsigned int port)
{
	int ret = 0;
	struct sockaddr_in serv_addr;

	server->sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (server->sockfd > 0) {
		VERBOSE("Binding and listening on port %u", port);
		ft_memset(&serv_addr, 0, sizeof(serv_addr));
		serv_addr.sin_family = AF_INET;
		serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
		serv_addr.sin_port = htons(port);

		if (-1 == bind(server->sockfd, (struct sockaddr *)&serv_addr,
				sizeof(serv_addr))) {
			perror("bind");
			ret = -1;
		}
		else if (-1 == listen(server->sockfd, MAX_CONN)) {
			perror("listen");
			ret = -1;
		}
		else {
			INFO("Server started on port %u", port);
		}
	}
	else {
		perror("socket");
		ret = -1;
	}
	return (ret);
}

int server_init(struct s_server *server, unsigned int port)
{
	int ret = -1;

	if (server)
	{
		VERBOSE("Initializing server");
		ft_memset(server, 0, sizeof(*server));

		init_methods(server);
		nickinit(&server->nicks);

		VERBOSE("Server initialized");
		ret = startup(server, port);
	}
	return (ret);
}
