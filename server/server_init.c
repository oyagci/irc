#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <stdlib.h>
#include <unistd.h>
#include "server.h"
#include "logger.h"

static int	init_methods(struct s_server *s)
{
	s->run = &loop;
	s->set_fds = &set_fds;
	s->accept = &accept_new_clients;
	s->read = &read_clients_command;
	s->send = &send_queued_replies;
	s->exec_cmd = &execute_command;
	s->queuecode = &queue_code_reply;
	s->queuenotif = &queue_reply;
	s->rm_from_chan = &rm_from_chan;
	s->get_channel = &get_channel;
	s->notifypart = &notifypart;
	s->new_channel = &new_channel;
	s->add_to_chan = &add_to_chan;
	return (0);
}

int			server_init(struct s_server *server, unsigned int port)
{
	struct sockaddr_in	serv_addr;

	ft_memset(server, 0, sizeof(*server));
	init_methods(server);
	if ((server->sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
		return (-1);
	ft_memset(&serv_addr, 0, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	serv_addr.sin_port = htons(port);
	if (-1 == bind(server->sockfd, (struct sockaddr *)&serv_addr,
		sizeof(serv_addr)))
	{
		LOG(LOGERR, "Could not create server on port %d", port);
		exit(EXIT_FAILURE);
	}
	if (-1 == listen(server->sockfd, MAX_CONN))
	{
		LOG(LOGERR, "Could not listen on port %d", port);
		exit(EXIT_FAILURE);
	}
	return (0);
}
