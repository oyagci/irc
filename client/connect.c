#include "client.h"

#include <sys/types.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <netdb.h>

#include <stdlib.h>
#include <errno.h>

int		client_connect(struct s_client *const self, 
					   struct s_client_msg const *const cmd)
{
	int					portno;
	struct addrinfo		*server = NULL;
	struct addrinfo		hints;

	if (self->servsock > 0)
	{
		LOG(LOGWARN, "Already connected");
		return (-1);
	}
	portno = ft_atoi(cmd->params[1]);

	LOG(LOGDEBUG, "Opening connection to %s:%d", cmd->params[0], portno);

	ft_bzero(&hints, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;

	int ret = 0;
	if ((ret = getaddrinfo(cmd->params[0], cmd->params[1], &hints, &server)) != 0)
	{
		LOG(LOGERR, "getaddrinfo: %s", gai_strerror(ret));
		exit(1);
		return (0);
	}
	if (-1 == (self->servsock = socket(server->ai_family, server->ai_socktype,
				server->ai_protocol)))
	{
		LOG(LOGERR, "Could not create socket (%s)", strerror(errno));
		exit(EXIT_FAILURE);
	}
	if (connect(self->servsock, server->ai_addr, server->ai_addrlen) < 0)
	{
		LOG(LOGERR, "Could not connect\n");
		printf(" * Could not connect to %s:%d (%s)\n", cmd->params[0], portno,
				strerror(errno));
	}
	return (0);
}
