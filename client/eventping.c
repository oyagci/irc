#include "client.h"

int	eventping(struct s_client *const self, struct s_message const *const msg)
{
	(void)msg;
	self->queuemsg(self, ft_strdup("PONG\x0d\x0a"));
	return (0);
}
