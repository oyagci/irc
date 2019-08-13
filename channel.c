#include "libft.h"
#include "server.h"
#include "logger.h"

int		channel_add_client(struct s_channel *channel, struct s_client *client)
{
	t_list	*elem;

	elem = ft_lstnew(0, 0);
	if (!elem)
		return (-1);
	elem->content = client;
	ft_lstpush(&channel->clients, elem);
	LOG(LOGDEBUG, "Client %s added to channel %.50s", client->nickname, channel->name);
	return (0);
}

int		channel_rm_nick(struct s_channel *const channel, char const *const nick)
{
	t_list			*prev;
	t_list			*clients;
	struct s_client	*client;

	clients = channel->clients;
	while (clients)
	{
		client = clients->content;
		if (ft_strnequ(client->nickname, nick, NICK_SIZE))
		{
			if (prev)
				prev->next = clients->next;
			else
				channel->clients = clients->next;
		}
		prev = clients;
		clients = clients->next;
	}
	return (0);
}
