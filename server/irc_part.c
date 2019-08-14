#include <stdlib.h>

#include "server.h"
#include "logger.h"

int				irc_part(struct s_client *client, char **params, int nparam)
{
	struct s_server		*s;
	struct s_channel	*chan;
	int					ret;

	s = client->server;
	if (nparam <= 0)
	{
		s->queuecode(s, client, ERR_NEEDMOREPARAM);
		return (0);
	}
	LOG(LOGDEBUG, "%s PART %s", client->nickname, params[0]);
	chan = s->get_channel(s, params[0]);
	if (chan)
	{
		ret = s->rm_from_chan(client->nickname, chan);
		if (ret == ERR_NOTONCHANNEL)
			s->queuecode(s, client, ERR_NOSUCHCHANNEL);
		else
			s->notifypart(s, chan, client->nickname);
	}
	else
		s->queuecode(s, client, ERR_NOSUCHCHANNEL);
	return (0);
}
