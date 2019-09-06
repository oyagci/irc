#include "libft.h"
#include "server.h"
#include <sys/socket.h>
#include <unistd.h>
#include <stdio.h>
#include "logger.h"
#include <stdlib.h>
#include <cbuf.h>

char	*irc_repcode_itoa(unsigned int n)
{
	char const	digits[10] = "0123456789";
	char		*s;
	int			i;

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

int	read_to_buffer(int cfd, t_cbuf_handle cbuf)
{
	int	ret;
	char buf[512];
	int	ii;
	int	complete;

	ft_memset(buf, 0, 512);
	if ((ret = recv(cfd, buf, 512, 0)) > 0)
	{
		LOG(LOGDEBUG, "Received %d bytes", ret);
		complete = 0;
		ii = 0;
		while (ii < ret)
		{
			if (cbuf_put2(cbuf, buf[ii]) < 0)
				break ;
			if (buf[ii] == '\n')
				complete += 1;
			ii += 1;
		}
		LOG(LOGDEBUG, "%d", ii);
	}
	return (complete);
}

int		read_clients_command(struct s_server *const self)
{
	t_list			*cur;
	struct s_client	*client;
	int				ii;
	char			buf[512];
	unsigned char	data;
	int				complete;

	cur = self->clients;
	while (cur)
	{
		client = cur->content;
		if (FD_ISSET(client->fd, &self->readfds))
		{
			complete = read_to_buffer(client->fd, client->cbuf);
			while (complete)
			{
				ii = 0;
				ft_memset(buf, 0, 512);
				while (ii < 512)
				{
					if (cbuf_get(client->cbuf, &data) < 0)
						break ;
					buf[ii] = data;
					if (buf[ii] == '\n')
						break ;
					ii += 1;
				}
				self->exec_cmd(client, buf);
				complete -= 1;
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

int			send_queued_replies(struct s_server *const server)
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
	t_rpl_handle const	handles[]  = { rpl_welcome, err_unknowncmd,
		err_unknowncmd, err_nickinuse };
	int const			replies[] = { RPL_WELCOME };
	char				*reply;
	size_t				i;

	reply = NULL;
	i = 0;
	if (reply_code > 0)
	{
		reply = ft_strnew(512);
		while (i < sizeof(handles) / sizeof(*handles))
		{
			if (replies[i] == reply_code)
				handles[i](reply, 512, c);
			i += 1;
		}
	}
	return (reply);
}

int			queue_reply(struct s_server *server,
	struct s_client const *const dest, char *reply)
{
	t_list				*elem;
	struct s_server_msg	*msg;

	msg = NULL;
	if (!(msg = ft_memalloc(sizeof(*msg))))
		return (-1);
	msg->dest = dest;
	ft_strncpy(msg->msg, reply, 512);
	msg->len = ft_strlen(reply);
	elem = ft_lstnew(0, 0);
	elem->content = msg;
	ft_lstadd(&server->msgqueue, elem);
	return (0);
}

int			queue_code_reply(struct s_server *server,
	struct s_client const *const dest, int reply_code)
{
	char				*replystr;

	replystr = server_format_reply(dest, reply_code);
	if (!replystr)
	{
		LOG(LOGWARN, "Reply code %d not handled", reply_code);
		return (-1);
	}
	server->queuenotif(server, dest, replystr);
	free(replystr);
	return (0);
}

void		server_msg_del(void *msgp, size_t size)
{
	struct s_server_msg *msg;

	(void)size;
	msg = msgp;
	free(msg);
}
