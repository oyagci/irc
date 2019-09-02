#include "client.h"

#include <sys/types.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <netdb.h>

#include <stdlib.h>
#include <errno.h>

static int	do_conn(struct s_client *self, char const *addr, char const *portstr,
	int portno)
{
	struct addrinfo		*server = NULL;
	struct addrinfo		hints;
	int					ret;
	int					gaierr;

	ret = -1;
	printf(" - Connecting to %s:%d\n", addr, portno);
	ft_bzero(&hints, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	if ((gaierr = getaddrinfo(addr, portstr, &hints, &server)) != 0)
		LOG(LOGERR, "getaddrinfo: %s", gai_strerror(gaierr));
	else if (-1 == (self->servsock = socket(server->ai_family,
				server->ai_socktype,
				server->ai_protocol)))
		LOG(LOGERR, "Could not create socket (%s)", strerror(errno));
	else if (connect(self->servsock, server->ai_addr, server->ai_addrlen) < 0)
		printf(" - Could not connect to %s:%d (%s)\n", addr, portno,
				strerror(errno));
	else
	{
		printf(" - Connected to %s\n", addr);
		ret = 0;
	}
	return (ret);
}

int			client_connect(struct s_client *const self, 
					   struct s_client_msg const *const cmd)
{
	unsigned short	portno;

	if (self->servsock > 0)
	{
		printf(" - Sorry, already connected\n");
		return (-1);
	}
	portno = ft_atoi(cmd->params[1]);
	return (do_conn(self, cmd->params[0], cmd->params[1], portno));
}
