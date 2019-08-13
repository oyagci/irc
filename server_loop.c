#include "server.h"

int	server_loop(struct s_server *self)
{
	int	max_sd;

	self->is_running = 1;
	while (self->is_running)
	{
		max_sd = self->set_fds(self);
		select(max_sd + 1, &self->readfds, &self->writefds, NULL, NULL);
		self->accept(self);
		server_read_clients_command(self);
		server_send_queued_replies(self);
	}
	return (0);
}
