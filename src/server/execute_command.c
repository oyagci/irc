#include "server.h"

int execute_command(struct s_server *self, struct s_client *c,
	char const *const cmd)
{
	struct {
		char const	*name;
		int (*f)(struct s_client *c, struct s_params *p);
	} const cmds[] = {
		{ .name = "PASS",    .f = irc_pass },
		{ .name = "NICK",    .f = irc_nick },
		{ .name = "USER",    .f = irc_user },
		{ .name = "JOIN",    .f = irc_join },
		{ .name = "PART",    .f = irc_part },
		{ .name = "WHO",     .f = irc_who },
		{ .name = "PONG",    .f = irc_pong },
		{ .name = "QUIT",    .f = irc_quit },
		{ .name = "PRIVMSG", .f = irc_privmsg }
	};
	struct s_message msg;
	int err = 0;
	int ret = 0;

	(void)self;
	ft_memset(&msg, 0, sizeof(msg));
	ret = message(&msg, cmd);
	if (ret >= 0) {
		for (size_t ii = 0; ii < sizeof(cmds) / sizeof(*cmds); ii++) {
			if (ft_strequ(cmds[ii].name, msg.cmd.data)) {
				err = cmds[ii].f(c, &msg.params);
				break;
			}
		}
		if (err) {
			ret = queue_code_reply(c->server, c, err);
		}
	}
	return (ret);
}
