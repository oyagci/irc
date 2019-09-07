#include "client.h"

int		client_leave(struct s_client *const self,
					 struct s_client_msg const *const cmd)
{
	char	*msg;

	if (!self->servsock)
	{
		printf(" - Not connected to a server\n");
		return (0);
	}
	msg = ft_strnew(513);
	if (!msg)
		return (-1);
	ft_strlcat(msg, "PART", 513);
	ft_strlcat(msg, " ", 513);
	ft_strlcat(msg, cmd->params[0], 513);
	ft_strlcat(msg, CRLF, 513);
	self->channel = 0;
	printf(" - Leaving channel %s\n", cmd->params[0]);
	return (self->queuemsg(self, msg));
}
