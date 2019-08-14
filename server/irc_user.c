#include "server.h"
#include "logger.h"

/*
**    Command: USER
** Parameters: <user> <mode> <unused> <realname>
*/
int	irc_user(struct s_client *c, char **params, int nparams)
{
	if (nparams < 4)
	{
		c->server->queuecode(c->server, c, ERR_NEEDMOREPARAM);
		return (ERR_NEEDMOREPARAM);
	}
	if (c->is_registered)
		return (ERR_ALREADYREGISTRED);
	c->username = ft_strdup(params[0]);
	LOG(LOGDEBUG, "Username of %.9s set to %s", c->nickname, c->username);
	set_usermode(c, ft_atoi(params[1]));
	set_realname(c, params[3]);
	LOG(LOGDEBUG, "Realname set to %s", c->realname);
	c->server->queuecode(c->server, c, RPL_WELCOME);
	return (0);
}
