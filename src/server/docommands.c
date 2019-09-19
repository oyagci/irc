#include <stdio.h>
#include "server.h"

static int docommand(struct s_server *self, struct s_client *client)
{
	char buf[512] = { 0 };
	unsigned char data = 0;
	int ii = 0;

	ft_memset(buf, 0, 512);
	ii = 0;
	while (ii < 512) {
		if (cbuf_get(client->cbuf, &data) < 0) {
			/* No more data, command is complete */
			break ;
		}

		buf[ii] = data;

		if (buf[ii] == '\n') {
			/* A newline means command is complete */
			break ;
		}
		ii += 1;
	}
	return (self->exec_cmd(self, client, buf));
}

int docommands(struct s_server *self)
{
	for (t_list *lst = self->clients; lst != NULL; lst = lst->next) {
		struct s_client *c = lst->content;

		while (c->ncmds > 0) {
			if (docommand(self, c) < 0) {
				fprintf(stderr, "%s:%d: Could not execute command\n",
					__FUNCTION__, __LINE__);
				/* TODO: Handle error */
			}
			c->ncmds -= 1;
		}
	}
	return (0);
}
