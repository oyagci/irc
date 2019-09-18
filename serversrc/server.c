/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oyagci <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/01 10:08:24 by oyagci            #+#    #+#             */
/*   Updated: 2019/09/09 14:27:41 by oyagci           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include "server.h"
#include "libft.h"

int execute_command(struct s_server *self, struct s_client *c,
	char const *const cmd)
{
	struct s_irc_cmds const cmds[] = {
		{ .name = "PASS", .f = irc_pass },
		{ .name = "NICK", .f = irc_nick },
		{ .name = "USER", .f = irc_user },
		{ .name = "JOIN", .f = irc_join },
		{ .name = "PART", .f = irc_part },
		{ .name = "WHO", .f = irc_who },
		{ .name = "PONG", .f = irc_pong },
		{ .name = "QUIT", .f = irc_quit },
		{ .name = "PRIVMSG", .f = irc_privmsg }
	};
	size_t ii = 0;
	int err = 0;
	struct s_message msg;

	ft_memset(&msg, 0, sizeof(msg));
	if (message(&msg, cmd) < 0)
		return (0);
	ii = 0;
	while (ii < 9) {
		if (ft_strequ(cmds[ii].name, msg.cmd.data)) {
			err = cmds[ii].f(c, &msg.params);
		}
		ii++;
	}
	if (!err) {
		c->server->queuecode(c->server, c, err);
	}
	self->update_clients(self);
	return (0);
}

int main(int ac, char *av[])
{
	int ret = 0;
	struct s_server server;

	if (ac < 2) {
		printf("Usage: %s <port>\n", av[0]);
		exit(EXIT_FAILURE);
	}
	ret = server_init(&server, ft_atoi(av[1]));
	if (!ret) {
		ret = server.run(&server);
	}
	return (ret);
}