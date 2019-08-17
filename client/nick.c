#include "client.h"

int		client_nick(struct s_client *const self,
					struct s_client_msg const *const cmd)
{
	char	*msg;
	char	*params[] = { "TODO", "*", "*", "Todo TODO" };
	struct s_client_msg	usercmd = {
		.cmd = CMD_USER,
		.nparam = 4,
		.params = params,
	};

	if (!self->servsock)
	{
		printf(" * Not connected to a server\n");
		return (0);
	}
	msg = ft_strnew(513);
	if (!msg)
		return (-1);
	ft_strlcat(msg, "NICK", 513);
	ft_strlcat(msg, " ", 513);
	ft_strlcat(msg, cmd->params[0], 513);
	ft_strlcat(msg, CRLF, 513);
	ft_memcpy(self->nickname, cmd->params[0], 9);
	self->queuemsg(self, msg);
	self->user(self, &usercmd);
	return (0);
}
