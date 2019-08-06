#include "libft.h"
#include "irc.h"
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdio.h>
#include "logger.h"

int		accept_new_clients(int sockfd, t_list **clients, fd_set *readfds)
{
	t_list					*elem;
	struct s_client			*client;
	struct sockaddr_in		cli_addr;
	socklen_t				cli_len;
	int						confd;

	cli_len = sizeof(cli_addr);
	if (FD_ISSET(sockfd, readfds)) {
		confd = accept(sockfd, (struct sockaddr *)&cli_addr, &cli_len);

		LOG(LOGDEBUG, "%s: connected", inet_ntoa(cli_addr.sin_addr));

		// Add connection to the clients list
		client = ft_memalloc(sizeof(*client));
		client->fd = confd;
		elem = ft_lstnew(NULL, 0);
		elem->content = client;
		ft_lstadd(clients, elem);
		LOG(LOGDEBUG, "New connection on fd %d", confd);
	}
	return (0);
}
