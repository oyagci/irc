#include <stdlib.h>
#include "server.h"

int	irc_who_client(struct s_server *server, struct s_client *client,
	struct s_client *dest)
{
	char	msg[512];
	char	*repcode;

	repcode = irc_repcode_itoa(RPL_WHOREPLY);
	ft_memset(msg, 0, 512);
	ft_strlcat(msg, repcode, 512);
	ft_strlcat(msg, " ", 512);
	ft_strlcat(msg, client->username, 512);
	ft_strlcat(msg, " ", 512);
	ft_strlcat(msg, "todo.42.fr", 512);
	ft_strlcat(msg, " ", 512);
	ft_strlcat(msg, "irc.42.fr", 512);
	ft_strlcat(msg, " ", 512);
	ft_strlcat(msg, client->nickname, 512);
	free(repcode);
	server->queuenotif(server, dest, msg);
	return (0);
}

int	irc_who(struct s_client *client, char **params, int nparam)
{
	t_list			*clients;
	struct s_client	*c;

	(void)params;
	(void)nparam;
	clients = client->server->clients;
	while (clients)
	{
		c = clients->content;
		irc_who_client(client->server, c, client);
		clients = clients->next;
	}
	return (0);
}
