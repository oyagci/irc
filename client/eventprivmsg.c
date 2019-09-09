#include "client.h"

int		eventprivmsg(struct s_client *const self,
	struct s_message const *const m)
{
	char	*from;
	char	*msg;
	char	*channel;

	(void)self;
	from = m->prefix->data;
	channel = m->params->param[0];
	msg = m->params->param[1];
	printf("[" BOLDRED "%s" RESET "] " UNDERLINE "%s" RESET ": %s\n",
			channel, from, msg);
	return (0);
}
