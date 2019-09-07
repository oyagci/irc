#include "client.h"
#include <stdlib.h>

int	eventpart(struct s_client *const self, struct s_message const *const cmd)
{
	char	*nick;
	char	*chan;

	nick = NULL;
	chan = NULL;
	nickname(cmd->prefix->data, &nick);
	chan = cmd->params->param[0];
	if (nick && chan)
	{
		self->channels.rmnick(&self->channels, nick, chan);
		printf(" * %s has left %s\n", nick, chan);
		free(nick);
	}
	return (0);
}
