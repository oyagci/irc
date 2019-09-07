#include "client.h"
#include <stdlib.h>

int		eventjoin(struct s_client *const self, struct s_message const *const m)
{
	char				*nick;
	int					ret;
	char const *const	chan = m->params->param[0];

	nick = NULL;
	ret = nickname(m->prefix->data, &nick);
	if (nick)
	{
		LOG(LOGDEBUG, "%s %s", nick, self->nickname);
		if (m->params && m->params->param[0])
		{
			self->channels.addnick(&self->channels, nick, chan);
			if (ft_strnequ(self->nickname, nick, 9))
			{
				self->channel = self->channels.get(&self->channels, m->params->param[0]);
			}
			printf(" * %s joined channel %s\n", nick, chan);
		}
		free(nick);
	}
	return (ret);
}
