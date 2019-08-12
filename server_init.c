#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <stdlib.h>
#include <unistd.h>
#include "server.h"
#include "logger.h"

int			server_init(struct s_server *server, unsigned int port)
{
	struct sockaddr_in	serv_addr;

	ft_memset(server, 0, sizeof(*server));
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
