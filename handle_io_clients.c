#include "libft.h"
#include "irc.h"
#include <sys/socket.h>
#include <unistd.h>
#include <stdio.h>
#include "logger.h"
#include <stdlib.h>

char	*irc_repcode_itoa(unsigned int n)
{
	char	digits[10] = "0123456789";
	char	*s;
	int		i;

	i = 0;
	s = ft_strdup("000");
	if (n > 999)
		return (NULL);
	while (n > 0)
	{
		s[2 - i] = digits[(n % 10)];
		n /= 10;
		i++;
	}
	return (s);
}

int		reply_client(struct s_client *c, int a)
{
	char	reply[512];
	char	*retstr;
	int		retcode;

	(void)a;
	retcode = c->lastret;
	if (retcode == 0) {
		return (0);
	}

	LOG(LOGDEBUG, "Replying to %d with code %03d", c->fd, retcode);
	reply[0] = 0;
	if (retcode > 0)
	{
		if (retcode == RPL_WELCOME)
		{
			retstr = irc_repcode_itoa(retcode);
			ft_strlcat(reply, ":irc.42.fr ", 512);
			ft_strlcat(reply, retstr, 512);
			ft_strlcat(reply, " ", 512);
			ft_strlcat(reply, "Welcome to the Internet Relay Chat, ", 512);
			ft_strlcat(reply, c->nickname, 512);
			ft_strlcat(reply, "!", 512);
			ft_strlcat(reply, CRLF, 512);
			send(c->fd, reply, ft_strlen(reply), 0);
			free(retstr);
		}
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
			LOG(LOGDEBUG, "Can't reply");
		cur = cur->next;
	}
	return (0);
}
