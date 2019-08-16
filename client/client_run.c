#include <unistd.h>
#include <sys/socket.h>
#include "client.h"

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
	char	*c;
	char	buf[512];

	if (FD_ISSET(self->servsock, &self->readfds))
	{
		ft_memset(buf, 0, 512);
		int	ret = recv(self->servsock, buf, 512, 0);
		if (ret > 0)
		{
			self->event(self, buf);
			c = ft_strstr(buf, CRLF);
			if (c)
				*c = '\0';
			LOG(LOGDEBUG, "%s", buf);
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

