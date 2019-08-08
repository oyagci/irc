#include "libft.h"
#include "irc.h"
#include "logger.h"

int		join_channel(struct s_client *client, char const *const chanstr)
{
	struct s_server *server;

	server = client->server;
	server_add_to_chan(server, client, chanstr);
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
		input += ret;
		LOG(LOGDEBUG, "-> '%s'", input);
		elem = ft_lstnew(0, 0);
		elem->content = chanbuf;
		ft_lstpush(&chans, elem);
		if (*input == ',')
			input++;
	}
	return (chans);
}

/* TODO */
int	irc_join(struct s_client *c, char **params, int nparams)
{
	t_list				*chans;
	struct s_channel	*chan;

	(void)c;
	chan = NULL;
	if (nparams < 1)
		return (ERR_NEEDMOREPARAM);
	chans = parse_param_channels(params[0]);
	for (t_list *l = chans; l != NULL; l = l->next)
	{
		char	*chan = l->content;

		join_channel(c, chan);
		LOG(LOGDEBUG, "%s joins channel %s", c->nickname, chan);
	}
	return (0);
}
