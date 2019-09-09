#include "client.h"
#include <sys/socket.h>
#include <errno.h>

static int	do_buffered_event(struct s_client *const client)
{
	char	buf[512];
	uint8_t	data;
	int		i;

	i = 0;
	ft_memset(buf, 0, 512);
	while (i < 512)
	{
		if (cbuf_get(client->cbuf, &data) < 0)
		{
			LOG(LOGDEBUG, "cbuf_get < 0");
			break ;
		}
		buf[i] = data;
		if (buf[i] == '\n')
			break ;
		i += 1;
	}
	return (client->event(client, buf));
}

static int	do_all_buffered_events(struct s_client *const client, int nevent)
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
		if (cbuf_put2(client->cbuf, buf[ii]) < 0)
		{
			LOG(LOGDEBUG, "buffer full");
			return (-1);
		}
		if (buf[ii] == '\n')
			nevent += 1;
		ii++;
	}
	return (nevent);
}

int			read_notif(struct s_client *const self)
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

