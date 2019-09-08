#include "libft.h"
#include "server.h"
#include "logger.h"
#include <stdlib.h>

/*
** Tell every client in the given channel that a new user joined,
** including the new user
*/
int	server_tell_new_client(struct s_server *server, struct s_client *client,
	struct s_channel *chan)
{
	t_list			*elem;
	char			*msg;
	struct s_client	*recipient;

	elem = chan->clients;
	while (elem)
	{
		recipient = elem->content;
		if (!(msg = ft_memalloc(sizeof(char) * 513)))
			exit(EXIT_FAILURE);
		ft_strlcat(msg, ":", 513);
		ft_strlcat(msg, client->nickname, 513);
		if (client->username)
		{
			ft_strlcat(msg, "!", 513);
			ft_strlcat(msg, client->username, 513);
		}
		ft_strlcat(msg, "@irc.42.fr", 513);
		ft_strlcat(msg, " JOIN ", 513);
		ft_strlcat(msg, chan->name, 513);
		ft_strlcat(msg, CRLF, 513);
		queue_reply(server, recipient, msg);
		free(msg);
		elem = elem->next;
	}
	return (0);
}

/*
** JOIN: Command sent by clients to join a list of channels
*/
int	irc_join(struct s_client *client, char **params, int nparams)
{
	t_list	*l;
	t_list	*chans;
	char	*chan;

	if (!client->is_registered)
		return (0);
	chan = NULL;
	if (nparams < 1)
		return (ERR_NEEDMOREPARAM);
	chans = channels(params[0]);
	l = chans;
	while (l)
	{
		chan = l->content;
		LOG(LOGDEBUG, "%s joins channel %s", client->nickname, chan);
		client->server->add_to_chan(client->server, client, chan);
		l = l->next;
	}
	channels_del(&chans);
	return (0);
}
