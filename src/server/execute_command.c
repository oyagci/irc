#include "server.h"

int execute_command(t_server *self, t_client *c, char const *const cmd)
{
	t_tuple_cmds const	cmds[] = {
		{ .name = "PASS", .f = irc_pass }, { .name = "NICK", .f = irc_nick },
		{ .name = "USER", .f = irc_user }, { .name = "JOIN", .f = irc_join },
		{ .name = "PART", .f = irc_part }, { .name = "WHO", .f = irc_who },
		{ .name = "PONG", .f = irc_pong }, { .name = "QUIT", .f = irc_quit },
		{ .name = "PRIVMSG", .f = irc_privmsg }
	};
	struct s_message	msg;
	int					err;
	int					ret;
	size_t				ii;

	(void)self;
	ft_memset(&msg, 0, sizeof(msg));
	err = 0;
	ret = message(&msg, cmd);
	if (ret >= 0)
	{
		ii = 0;
		while (ii < sizeof(cmds) / sizeof(*cmds))
		{
			if (ft_strequ(cmds[ii].name, msg.cmd.data))
			{
				err = cmds[ii].f(c, &msg.params);
				break;
			}
			ii++;
		}
		if (err)
			ret = queue_code_reply(c->server, c, err);
	}
	return (ret);
}
