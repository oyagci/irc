#include <unistd.h>
#include <sys/socket.h>
#include "client.h"
#include <cbuf.h>
#include "logger.h"

int		read_input(struct s_client *self)
{
	char				buf[512];
	struct s_client_msg	*cmd;
	char				*c;

	if (FD_ISSET(0, &self->readfds))
	{
		fgets(buf, 512, stdin);
		if ((c = ft_strchr(buf, '\n')))
			*c = '\0';
		cmd = self->parse_input(self, buf);
		self->exec_cmd(self, cmd);
	}
	return (0);
}

int		read_notif(struct s_client *self)
{
	char	buf[512];
	int		complete;
	int		ret;
	int		i;

	if (FD_ISSET(self->servsock, &self->readfds))
	{
		ft_memset(buf, 0, 512);
		if ((ret = recv(self->servsock, buf, 512, 0)) > 0)
		{
			LOG(LOGDEBUG, "Received %d bytes", ret);
			i = 0;
			complete = 0;
			while (i < ret)
			{
				cbuf_put2(self->cbuf, buf[i]);
				if (buf[i] == '\n')
					complete += 1;
				i++;
			}
			while (complete)
			{
				char	buf2[512];
				unsigned char	data;

				i = 0;
				ft_memset(buf2, 0, 512);
				while (i < 512 && buf2[i] != 0x0d)
				{
					if (cbuf_get(self->cbuf, &data) < 0)
						break ;
					buf2[i] = data;
					i += 1;
				}
				self->event(self, buf2);
				complete -= 1;
			}
		}
	}
	return (0);
}

int		client_run(struct s_client *self)
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
		select(self->servsock + 1, &self->readfds, NULL, NULL, &t);
		read_notif(self);
		read_input(self);
		self->sendmsgs(self);
	}
	return (0);
}

