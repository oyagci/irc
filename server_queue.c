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

int		server_read_clients_command(struct s_server const *const server, t_list *clients)
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
		cur = cur->next;
	}
	return (0);
}

static int	server_reply_to_client(struct s_server_msg const *const msg)
{
	LOG(LOGDEBUG, "%s", msg->msg);
	return (send(msg->dest->fd, msg->msg, msg->len, 0));
}

int			server_send_queued_replies(struct s_server *const server)
{
	t_list				*prev;
	t_list				*next;
	t_list				*msgelem;
	struct s_server_msg	*msg;

	prev = NULL;
	next = NULL;
	msgelem = server->msgqueue;
	while (msgelem)
	{
		next = msgelem->next;
		msg = msgelem->content;
		if (FD_ISSET(msg->dest->fd, &server->writefds))
		{
			/* TODO: Error handling */
			server_reply_to_client(msg);

			if (prev)
				prev->next = msgelem->next;
			else
				server->msgqueue = msgelem->next;
			ft_lstdelone(&msgelem, &server_msg_del);
		}
		prev = msgelem;
		msgelem = next;
	}
	return (0);
}

static char	*server_format_reply(struct s_client const *const c, int reply_code)
{
	char	*reply;
	char	*retstr;

	if (reply_code == 0)
		return (NULL);

	reply = NULL;
	if (reply_code > 0)
	{
		reply = ft_strnew(512);
		retstr = irc_repcode_itoa(reply_code);
		if (retstr)
		{
			if (reply_code == RPL_WELCOME)
			{
				ft_strlcat(reply, ":irc.42.fr ", 512);
				ft_strlcat(reply, retstr, 512);
				ft_strlcat(reply, " ", 512);
				ft_strlcat(reply, "Welcome to the Internet Relay Chat, ", 512);
				ft_strlcat(reply, c->nickname, 512);
				ft_strlcat(reply, "!", 512);
				ft_strlcat(reply, CRLF, 512);
			}
			else if (reply_code == ERR_UNKNOWNCOMMAND)
			{
				ft_strlcat(reply, ":irc.42.fr ", 512);
				ft_strlcat(reply, retstr, 512);
				ft_strlcat(reply, " ", 512);
				ft_strlcat(reply, "Unknown command", 512);
			}
			else
				ft_strdel(&reply);
			free(retstr);
		}
		else
			ft_strdel(&reply);
	}
	return (reply);
}

int			server_queue_reply(struct s_server *server, struct s_client const *const dest,
	char *reply)
{
	t_list				*elem;
	struct s_server_msg	*msg;


	msg = NULL;
	if (!(msg = ft_memalloc(sizeof(*msg))))
		return (-1);
	msg->dest = dest;
	msg->msg = reply;
	msg->len = ft_strlen(reply);

	elem = ft_lstnew(0, 0);
	elem->content = msg;
	ft_lstadd(&server->msgqueue, elem);
	LOG(LOGDEBUG, "%s", reply);
	return (0);
}

int			server_queue_code_reply(struct s_server *server,
	struct s_client const *const dest, int reply_code)
{
	char				*replystr;

	replystr = server_format_reply(dest, reply_code);
	if (!replystr)
		return (-1);
	server_queue_reply(server, dest, replystr);
	return (0);
}

void		server_msg_del(void *msgp, size_t size)
{
	struct s_server_msg *msg;

	(void)size;
	msg = msgp;
	free(msg->msg);
	free(msg);
}
