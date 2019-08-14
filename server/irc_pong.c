#include "server.h"
#include <time.h>

int		irc_pong(struct s_client *client, char **params, int nparam)
{
	struct timespec	current;

	(void)params;
	(void)nparam;
	clock_gettime(CLOCK_REALTIME, &current);
	client->ping.tv_sec = current.tv_sec + 10;
	client->timeout.tv_sec = current.tv_sec;
	return (0);
}
