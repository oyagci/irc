#include "server.h"
#include <time.h>
#include "logger.h"

int		updatetimeout(struct s_server *self)
{
	(void)self;
	return (0);
}

int		pinginactive(struct s_server *self)
{
	t_list			*clients;
	struct s_client	*client;
	struct timespec	current;

	clients = self->clients;
	while (clients)
	{
		client = clients->content;
		clock_gettime(CLOCK_REALTIME, &current);
		if (current.tv_sec == client->ping.tv_sec)
		{
			irc_ping(client, 0, 0);
			clock_gettime(CLOCK_REALTIME, &client->ping);
			client->ping.tv_sec += 10;
		}
		clients = clients->next;
	}
	return (0);
}
