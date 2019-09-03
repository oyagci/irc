#include "server.h"
#include "logger.h"

int	irc_nick(struct s_client *c, char **params, int nparams)
{
	char	*nick;

	if (nparams < 1)
	{
		c->server->queuecode(c->server, c, ERR_NONICKNAMEGIVEN);
		return (ERR_NONICKNAMEGIVEN);
	}
	nick = params[0];
	if (ft_strlen(nick) > 9)
	{
		LOG(LOGDEBUG, "Nickname %.9s is too long (len > 9)", nick);
		c->server->queuecode(c->server, c, ERR_ERRONEUSNICKNAME);
		return (ERR_ERRONEUSNICKNAME);
	}
	if (!nickadd(&c->server->nicks, nick))
	{
		LOG(LOGDEBUG, "Nickname %.9s is already in use", nick);
		c->server->queuecode(c->server, c, ERR_NICKNAMEINUSE);
		return (ERR_NICKNAMEINUSE);
	}
	LOG(LOGDEBUG, "NICK %.9s -> %.9s", c->nickname, nick);
	strncpy(c->nickname, nick, NICK_SIZE);
	return (0);
}

