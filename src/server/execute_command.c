#include "server.h"

int execute_command(t_server *self, t_client *c, char const *const cmd)
{
	t_tuple_cmds const	cmds[] = {
		{ "PASS", irc_pass }, { "NICK", irc_nick }, { "USER", irc_user },
		{ "JOIN", irc_join }, { "PART", irc_part }, { "WHO", irc_who },
		{ "PONG", irc_pong }, { "QUIT", irc_quit }, { "PRIVMSG", irc_privmsg }
	};
	struct s_message	msg;
	int					err;
	int					ret;
	size_t				ii;

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
			ret = queue_code_reply(self, c, err);
	}
	return (ret);
}
