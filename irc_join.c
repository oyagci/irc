#include "libft.h"
#include "irc.h"
#include "logger.h"
#include <assert.h>

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

int	server_tell_new_client(struct s_server *server, struct s_client *client,
		struct s_channel *chan)
{
	char	*msg;

	msg = ft_memalloc(sizeof(char) * 513);
	ft_strlcat(msg, ":", 513);
	ft_strlcat(msg, client->nickname, 513);
	ft_strlcat(msg, "!", 513);
	ft_strlcat(msg, client->username, 513);
	ft_strlcat(msg, "@irc.42.fr", 513);
	ft_strlcat(msg, " JOIN ", 513);
	ft_strlcat(msg, chan->name, 513);
	ft_strlcat(msg, CRLF, 513);
	server_queue_reply(server, client, msg);
	return (0);
}

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
	LOG(LOGDEBUG, "Channel %s has been created", chan->name);
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

int		join_channel(struct s_client *client, char const *const chanstr)
{
	struct s_server *server;

	server = client->server;
	return (server_add_to_chan(server, client, chanstr));
}

int		channel(char const *inputc, char **buffer)
{
	unsigned char const	*input;
	int					ii;

	input = (unsigned char const *)inputc;
	if (*input == '#' || *input == '+' || *input == '&')
	{
		ii = 1;
		while (input[ii] != '\0' &&
				((input[ii] >= 0x01 && input[ii] <= 0x07) ||
				 (input[ii] >= 0x08 && input[ii] <= 0x09) ||
				 (input[ii] >= 0x0b && input[ii] <= 0x0c) ||
				 (input[ii] >= 0x0e && input[ii] <= 0x1f) ||
				 (input[ii] >= 0x2d && input[ii] <= 0x39) ||
				 (input[ii] >= 0x3b)))
		{
			ii++;
		}
		if (ii > 1 && ii <= 50)
		{
			*buffer = ft_strndup((char *)inputc, ii);
			return (ii);
		}
	}
	return (0);
}

t_list		*parse_param_channels(char *input)
{
	t_list	*chans;
	t_list	*elem;
	int		ret;
	char	*chanbuf;

	chans = NULL;
	while ((ret = channel(input, &chanbuf)) > 0)
	{
		elem = ft_lstnew(0, 0);
		elem->content = chanbuf;
		ft_lstpush(&chans, elem);
		input += ret;
		if (*input == ',')
			input++;
	}
	return (chans);
}

/* TODO */
int	irc_join(struct s_client *c, char **params, int nparams)
{
	t_list	*chans;
	char	*chan;

	(void)c;
	chan = NULL;
	if (nparams < 1)
		return (ERR_NEEDMOREPARAM);
	chans = parse_param_channels(params[0]);
	for (t_list *l = chans; l != NULL; l = l->next)
	{
		chan = l->content;
		LOG(LOGDEBUG, "%s joins channel %s", c->nickname, chan);
		join_channel(c, chan);
	}
	return (0);
}
