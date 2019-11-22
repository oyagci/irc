#include <stdio.h>
#include "server.h"

static int docommand(struct s_server *self, struct s_client *client)
{
	char			buf[512];
	unsigned char	data;
	int				ii;

	data = 0;
	ft_memset(buf, 0, 512);
	ii = 0;
	while (ii < 512)
	{
		if (cbuf_get(client->cbuf, &data) < 0)
			break ;
		buf[ii] = data;
		if (buf[ii] == '\n')
			break ;
		ii += 1;
	}
	return (execute_command(self, client, buf));
}

int docommands(struct s_server *self)
{
	t_client	*c;
	size_t		i;

	i = 0;
	while (i < self->nclients)
	{
		c = self->clients + i;
		while (c->ncmds > 0)
		{
			if (docommand(self, c) < 0)
			{
				fprintf(stderr, "%s:%d: Could not execute command\n",
					__FUNCTION__, __LINE__);
				/* TODO: Handle error */
			}
			c->ncmds -= 1;
		}
		i += 1;
	}
	return (0);
}
