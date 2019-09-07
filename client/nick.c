#include "client.h"

int		client_nick(struct s_client *const self,
					struct s_client_msg const *const cmd)
{
	char				*msg;

	ft_strncpy(self->nickname, cmd->params[0], SNICK);
	msg = ft_strnew(513);
	if (!msg)
		return (-1);
	ft_strlcat(msg, "NICK", 513);
	ft_strlcat(msg, " ", 513);
	ft_strlcat(msg, cmd->params[0], 513);
	ft_strlcat(msg, CRLF, 513);
	if (self->servsock > 0)
		self->queuemsg(self, msg);
	return (0);
}
