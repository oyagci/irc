#include <unistd.h>
#include <sys/socket.h>
#include "client.h"
#include <cbuf.h>
#include "logger.h"
#include <errno.h>

static int	read_input(struct s_client *self)
{
	char				buf[512 + 1];
	struct s_client_msg	*cmd;
	char				*c;
	int					ret;

	ret = 0;
	buf[512] = 0;
	if (FD_ISSET(0, &self->readfds))
	{
		ret = read(0, buf, 512);
		if (ret >= 0)
		{
			if ((c = ft_strchr(buf, '\n')))
				*c = '\0';
			cmd = self->parse_input(self, buf);
			self->exec_cmd(self, cmd);
		}
	}
	return (ret);
}

static int	do_buffered_event(struct s_client *client)
{
	char	buf[512];
	uint8_t	data;
	int		i;

	i = 0;
	ft_memset(buf, 0, 512);
	while (i < 512)
	{
		if (cbuf_get(client->cbuf, &data) < 0)
			break ;
		buf[i] = data;
		if (buf[i] == '\n')
			break ;
		i += 1;
	}
	return (client->event(client, buf));
}

static int	do_all_buffered_events(struct s_client *client, int nevent)
{
	int	ret;

	ret = 0;
	while (nevent)
	{
		if (!do_buffered_event(client))
			ret += 1;
		nevent -= 1;
	}
	return (ret);
}

static int	bufferise_events(struct s_client *client, char *buf, int buflen)
{
	int	ii;
	int	nevent;

	nevent = 0;
	ii = 0;
	while (ii < buflen)
	{
		/* TODO: Handle full buffer */
		if (cbuf_put2(client->cbuf, buf[ii] < 0))
			return (-1);
		if (buf[ii] == '\n')
			nevent += 1;
		ii++;
	}
	return (0);
}

static int	read_notif(struct s_client *self)
{
	char	buf[512 + 1];
	int		nevent;
	int		ret;

	buf[512] = 0;
	if (self->servsock > 0 && FD_ISSET(self->servsock, &self->readfds))
	{
		if ((ret = recv(self->servsock, buf, 512, 0)) > 0)
		{
			nevent = bufferise_events(self, buf, ret);
			do_all_buffered_events(self, nevent);
		}
		else if (ret < 0)
		{
			LOG(LOGERR, "recv has failed (%s)", strerror(errno));
			return (-1);
		}
	}
	return (0);
}

int			client_run(struct s_client *self)
{
	struct timeval		t;

	self->is_running = 1;
	while (self->is_running)
	{
		FD_ZERO(&self->readfds);
		FD_SET(self->servsock, &self->readfds);
		FD_SET(STDIN_FILENO, &self->readfds);
		FD_ZERO(&self->writefds);
		FD_SET(self->servsock, &self->writefds);
		t.tv_sec = 0;
		t.tv_usec = 10000;
		if (select(self->servsock + 1, &self->readfds, NULL, NULL, &t) < 0)
		{
			LOG(LOGERR, "select has failed");
			return (-1);
		}
		if (read_notif(self) < 0 || read_input(self) < 0)
			return (-1);
		self->sendmsgs(self);
	}
	return (0);
}

