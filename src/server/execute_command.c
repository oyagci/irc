/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_command.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oyagci <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/22 12:51:51 by oyagci            #+#    #+#             */
/*   Updated: 2019/11/25 12:32:42 by oyagci           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "server.h"

static int	(*get_cmd(char *name))(t_client *c, struct s_params *p)
{
	t_tuple_cmds const	cmds[] = {
		{ "NICK", irc_nick }, { "USER", irc_user }, { "JOIN", irc_join },
		{ "PART", irc_part }, { "WHO", irc_who }, { "QUIT", irc_quit },
		{ "PRIVMSG", irc_privmsg }
	};
	size_t				ii;

	ii = 0;
	while (ii < sizeof(cmds) / sizeof(*cmds))
	{
		if (ft_strequ(cmds[ii].name, name))
			return (cmds[ii].f);
		ii++;
	}
	return (NULL);
}

int			execute_command(t_server *self, t_client *c, char const *const cmd)
{
	struct s_message	msg;
	int					err;
	int					ret;
	int					(*p)(t_client *c, struct s_params *p);

	ft_memset(&msg, 0, sizeof(msg));
	err = 0;
	ret = message(&msg, cmd);
	if (ret < 0)
		return (ret);
	p = get_cmd(msg.cmd.data);
	if (p)
		err = p(c, &msg.params);
	if (err)
		ret = queue_code_reply(self, c, err);
	return (ret);
}
