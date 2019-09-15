#include "client.h"
#include <stdio.h>

int	whoreply(struct s_client *const self,
	struct s_message const *const msg)
{
	size_t i;

	(void)self;
	i = 0;
	while (i < msg->params.nparam)
	{
		printf("%s ", msg->params.param[i]);
		i += 1;
	}
	printf("\n");
	return (0);
}
