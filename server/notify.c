#include "server.h"

int			server_notifypart(struct s_server *s, struct s_channel *chan,
	char const *const nick)
{
	t_list			*elem;
	struct s_client	*c;
	char			notif[513];

	ft_memset(notif, 0, 513);
	elem = chan->clients;
	ft_strlcat(notif, ":", 513);
	ft_strlcat(notif, nick, 513);
	ft_strlcat(notif, " ", 513);
	ft_strlcat(notif, "PART", 513);
	ft_strlcat(notif, " ", 513);
	ft_strlcat(notif, chan->name, 513);
	ft_strlcat(notif, CRLF, 513);
	while (elem)
	{
		c = elem->content;
		s->queuenotif(s, c, notif);
		elem = elem->next;
	}
	return (0);
}
