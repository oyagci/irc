#include "libft.h"
#include "server.h"
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdio.h>
#include "logger.h"
#include <time.h>

int		setnosigpipe(int fd)
{
	int	set;

	set = 1;
	return (setsockopt(fd, SOL_SOCKET, SO_NOSIGPIPE, (void *)&set, sizeof(set)));
}

int		accept_new_clients(struct s_server *server)
{
	t_list					*elem;
	struct s_client			*client;
	struct sockaddr_in		cli_addr;
	socklen_t				cli_len;
	int						confd;

	cli_len = sizeof(cli_addr);
	if (FD_ISSET(server->sockfd, &server->readfds))
	{
		confd = accept(server->sockfd, (struct sockaddr *)&cli_addr, &cli_len);
		setnosigpipe(confd);
		LOG(LOGDEBUG, "%s: connected on fd %d", inet_ntoa(cli_addr.sin_addr),
			confd);
		client = ft_memalloc(sizeof(*client));
		client->fd = confd;
		client->server = server;
		clock_gettime(CLOCK_REALTIME, &client->ping);
		clock_gettime(CLOCK_REALTIME, &client->timeout);
		client->ping.tv_sec += 10;
		ft_memcpy(client->nickname, "guest", 5);
		elem = ft_lstnew(NULL, 0);
		elem->content = client;
		ft_lstadd(&server->clients, elem);
	}
	return (0);
}
