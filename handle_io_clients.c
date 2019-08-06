#include "libft.h"
#include "irc.h"
#include <sys/socket.h>
#include <unistd.h>
#include <stdio.h>

int		reply_client(struct s_client *c, int __unused a)
{
	char	*retstr;
	int		retcode;

	retcode = c->lastret;
	if (retcode == 0)
		return (0);

	printf("Replying to %d with code %03d\n", c->fd, retcode);
	if (retcode >= 401 && retcode <= 502)
	{
		retstr = ft_itoa(retcode);
		send(c->fd, retstr, 3, 0);
	}
	c->lastret = 0;
	return (0);
}

int		handle_io_clients(struct s_server const *const server, t_list *clients)
{
	t_list			*cur;
	struct s_client	*client;

	cur = clients;
	while (cur)
	{
		client = cur->content;
		if (FD_ISSET(client->fd, &server->readfds))
		{
			read_client_command(client->fd, &client->buffer);
			if (client->buffer.is_complete)
			{
				client->buffer.is_complete = 0;
				client->buffer.len = 0;
				client->lastret = execute_command(client);
				ft_memset(client->buffer.data, 0, CLIENT_BUFFER_SIZE);
			}
		}
		if (FD_ISSET(client->fd, &server->writefds))
		{
			reply_client(client, 0);
		}
		else
			printf("Can't reply\n");
		cur = cur->next;
	}
	return (0);
}
