#include "irc.h"
#include "libft/includes/libft.h"
#include "logger.h"

struct s_channel	*server_new_channel(struct s_server *server, char const *name, int mode)
{
	t_list				*elem;
	struct s_channel	*chan;

	elem = server->channels;
	while (elem)
	{
		chan = elem->content;
		if (ft_strequ(chan->name, name))
			return (NULL);
	}
	chan = ft_memalloc(sizeof(*chan));
	if (!chan)
		return (NULL);
	ft_strcpy(chan->name, name);
	chan->mode = mode;
	elem = ft_lstnew(0, 0);
	elem->content = chan;
	ft_lstpush(&server->channels, elem);
	LOG(LOGDEBUG, "Channel %s has been created", chan->name);
	return (chan);
}

struct s_channel	*server_get_channel(struct s_server *server, char const *name)
{
	t_list				*elem;
	struct s_channel	*chan;

	elem = server->channels;
	chan = NULL;
	while (elem)
	{
		chan = elem->content;
		if (ft_strequ(chan->name, name))
			break ;
		chan = NULL;
		elem = elem->next;
	}
	return (chan);
}

int	server_add_to_chan(struct s_server *server, struct s_client *client,
	char const *const channame)
{
	t_list				*elem;
	struct s_channel	*chan;

	elem = server->channels;
	chan = NULL;
	while (elem)
	{
		chan = elem->content;
		if (ft_strequ(channame, chan->name))
			break ;
		chan = NULL;
		elem = elem->next;
	}
	if (!chan)
	{
		LOG(LOGDEBUG, "No channel named '%s' found. Creating channel...", channame);
		chan = server_new_channel(server, channame, 0);
	}
	else
		LOG(LOGDEBUG, "Channel %.50s found", chan->name);
	channel_add_client(chan, client);
	server_tell_new_client(server, client, chan);
	return (0);
}