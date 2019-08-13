#include "libft.h"
#include "server.h"
#include "logger.h"

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
		msg = ft_memalloc(sizeof(char) * 513);
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
		server_queue_reply(server, recipient, msg);
		elem = elem->next;
	}
	return (0);
}

/*
** JOIN: Command sent by clients to join a list of channels
*/
int	irc_join(struct s_client *client, char **params, int nparams)
{
	t_list	*chans;
	char	*chan;

	chan = NULL;
	if (nparams < 1)
		return (ERR_NEEDMOREPARAM);
	chans = channels(params[0]);
	for (t_list *l = chans; l != NULL; l = l->next)
	{
		chan = l->content;
		LOG(LOGDEBUG, "%s joins channel %s", client->nickname, chan);
		server_add_to_chan(client->server, client, chan);
	}
	return (0);
}
