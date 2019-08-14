#include "server.h"
#include "logger.h"

int		quit(struct s_server *self, struct s_client *client, char const *const msg)
{
	char	*notif;

	notif = ft_strnew(512);
	ft_strlcat(notif, "QUIT", 512);
	ft_strlcat(notif, " :", 512);
	ft_strlcat(notif, msg, 512);
	self->queuenotif(self, client, notif);
	LOG(LOGDEBUG, "QUIT");
	return (0);
}
