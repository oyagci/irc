#include "client.h"

int		quit(struct s_client *const self,
	struct s_client_msg const *const clientmsg)
{
	char	*msg;

	msg = ft_strnew(512);
	if (!msg)
		return (-1);
	ft_strlcat(msg, "QUIT :", 513);
	if (clientmsg->nparam > 0)
	{
		ft_strlcat(msg, clientmsg->params[clientmsg->nparam - 1], 513);
	}
	ft_strlcat(msg, CRLF, 513);
	self->queuemsg(self, msg);
	return (0);
}
