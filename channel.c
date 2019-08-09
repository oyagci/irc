#include "libft.h"
#include "irc.h"
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
