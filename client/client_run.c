#include <unistd.h>
#include <sys/socket.h>
#include "client.h"
#include <cbuf.h>
#include "logger.h"
#include <errno.h>
#include "libft.h"
#include <stdlib.h>

static int	read_input(struct s_client *const self)
{
	char				buf[512 + 1];
	struct s_client_msg	*cmd;
	char				*c;
	int					ret;

	ret = 0;
	buf[512] = 0;
	ft_memset(buf, 0, sizeof(buf) / sizeof(*buf));
	if (FD_ISSET(0, &self->readfds))
	{
		ret = read(0, buf, 512);
		if (ret >= 0)
		{
			if ((c = ft_strchr(buf, '\n')))
				*c = '\0';
			if (ft_strlen(buf) == 0)
				return (0);
			if ((cmd = self->parse_input(self, buf)))
			{
				self->exec_cmd(self, cmd);
				free(cmd);
			}
		}
	}
	return (ret);
}

int			client_run(struct s_client *const self)
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
