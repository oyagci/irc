#include "server.h"
#include "logger.h"
#include <stdlib.h>
#include <unistd.h>

void	removeclient(struct s_server *self, struct s_client *client)
{
	t_list	*chans;
	struct s_channel	*c;

	chans = self->channels;
	while (chans)
	{
		c = chans->content;
		channel_rm_nick(c, client->nickname);
		chans = chans->next;
	}
	close(client->fd);

	t_list	*clients;
	t_list	*prev;

	prev = 0;
	clients = self->clients;
	while (clients)
	{
		struct s_client	*ci;

		ci = clients->content;
		if (ft_strnequ(client->nickname, ci->nickname, 9))
		{
			if (prev)
				prev->next = clients->next;
			else
				self->clients = clients->next;

			free(ci);
		}
		prev = clients;
		clients = clients->next;
	}
}

int		quit(struct s_server *self, struct s_client *client,
	char const *const msg)
{
	char	*notif;

	notif = ft_strnew(512);
	ft_strlcat(notif, "QUIT", 512);
	ft_strlcat(notif, " :", 512);
	ft_strlcat(notif, msg, 512);
	removeclient(self, client);
	self->queuenotif(self, client, notif);
	return (0);
}
