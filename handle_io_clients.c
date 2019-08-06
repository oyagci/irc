#include "libft.h"
#include "irc.h"
#include <sys/socket.h>

int		reply_client(struct s_client *c, int retcode)
{
	(void)c;
	(void)retcode;
	return (0);
}

int		handle_io_clients(struct s_server const *const server, t_list *clients)
{
	t_list			*cur;
	struct s_client	*client;
	int				cmdret;

	cmdret = 0;
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
				cmdret = execute_command(client);
				reply_client(client, cmdret);
				ft_memset(client->buffer.data, 0, CLIENT_BUFFER_SIZE);
			}
		}
		cur = cur->next;
	}
	return (0);
}
