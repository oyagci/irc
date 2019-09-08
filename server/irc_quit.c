#include "server.h"
#include <libft.h>
#include <unistd.h>
#include "logger.h"

void	remove_from_all_channels(struct s_client *const c)
{
	for (t_list *l = c->server->channels; l != NULL; l = l->next)
	{
		struct s_channel	*chan;

		chan = l->content;
		rm_from_chan(c->nickname, chan);
		c->server->notifypart(c->server, chan, c->nickname);
	}
}

int	irc_quit(struct s_client *c, char **params, int nparams)
{
	LOG(LOGDEBUG, "QUIT");
	(void)params;
	(void)nparams;
	remove_from_all_channels(c);
	nickremove(&c->server->nicks, c->nickname);
	close(c->fd);
	return (0);
}
