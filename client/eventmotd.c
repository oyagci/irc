#include "client.h"

int	eventmotd(struct s_client *const self, struct s_message const *const cmd)
{
	(void)self;
	printf("%s\n", cmd->params->param[cmd->params->nparam - 1]);
	return (0);
}
