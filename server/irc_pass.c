#include "server.h"

int	irc_pass(struct s_client *c, char **params, int nparams)
{
	if (c->is_connected)
	{
		c->server->queuecode(c->server, c, ERR_ALREADYREGISTRED);
		return (ERR_ALREADYREGISTRED);
	}
	if (SERVER_PASS)
	{
		if (nparams < 1)
		{
			c->server->queuecode(c->server, c, ERR_NEEDMOREPARAM);
			return (ERR_NEEDMOREPARAM);
		}
		if (ft_strcmp(params[0], SERVER_PASS))
		{
			c->server->queuecode(c->server, c, ERR_PASSWDMISMATCH);
			return (ERR_PASSWDMISMATCH);
		}
	}
	c->is_connected = 1;
	return (0);
}
