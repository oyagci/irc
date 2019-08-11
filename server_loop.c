#include "irc.h"

int	server_loop(struct s_server *server)
{
	int	max_sd;

	server->is_running = 1;
	while (server->is_running)
	{
		max_sd = server_set_fds(server);
		select(max_sd + 1, &server->readfds, &server->writefds, NULL, NULL);
		server_accept_new_clients(server);
		server_read_clients_command(server);
		server_send_queued_replies(server);
	}
	return (0);
}
