#include <time.h>
#include "server.h"

int		irc_ping(struct s_client *client, char **params, int nparam)
{
	char			pingmsg[512];
	struct s_server	*s;

	(void)params;
	(void)nparam;
	s = client->server;
	ft_strlcat(pingmsg, "PING", 512);
	ft_strlcat(pingmsg, CRLF, 512);
	s->queuenotif(s, client, pingmsg);
	return (0);
}
