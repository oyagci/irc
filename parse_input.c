#include "server.h"
#include "client.h"
#include <stdio.h>
#include "logger.h"
#include "libft.h"

enum e_client_cmd	set_command(char **inputp, struct s_client_msg *buf)
{
	const struct s_client_cmds	cmds[] = {
		{ .str = "connect", .cmd = CMD_CONNECT, .nparam = 2 },
		{ .str = "nick", .cmd = CMD_NICK, .nparam = 1 },
		{ .str = "join", .cmd = CMD_JOIN, .nparam = 1 },
		{ .str = "leave", .cmd = CMD_LEAVE, .nparam = 1 },
		{ .str = "msg", .cmd = CMD_MSG, .nparam = 2 },
		{ .str = "disconnect", .cmd = CMD_DISCONNECT, .nparam = 0 },
		{ .str = "who", .cmd = CMD_WHO, .nparam = 1 },
	};
	char						*input;
	int							ii;
	size_t						jj;

	input = *inputp;
	ii = 0;
	while (input[ii] && input[ii] != ' ')
		ii += 1;
	jj = 0;
	while (jj < sizeof(cmds) / sizeof(*cmds))
	{
		if (ft_strnequ(input, cmds[jj].str, ii - 1))
		{
			*inputp += ii;
			buf->cmd = cmds[jj].cmd;
			buf->nparam = cmds[jj].nparam;
			return (cmds[jj].cmd);
		}
		jj += 1;
	}
	return (CMD_NONE);
}

int					set_params(char *input, struct s_client_msg *buf)
{
	int		start;
	int		end;
	size_t	ii;

	if (*input == 0)
		return (0);
	buf->params = ft_memalloc(sizeof(char *) * buf->nparam);
	start = 0;
	end = 0;
	ii = 0;
	LOG(LOGDEBUG, "buf->nparam %ld", buf->nparam);
	if (buf->nparam > 0)
	{
		while (input[end] != '\0' && ii < buf->nparam - 1)
		{
			end = start;
			while (input[end] != ' ' && input[end] != '\0')
				end++;
			buf->params[ii] = ft_strndup(input + start, end - start);
			LOG(LOGDEBUG, "param: %s", buf->params[ii]);
			if (input[end] == ' ')
				end++;
			start = end;
			ii += 1;
		}
		buf->params[ii] = ft_strdup(input + start);
		LOG(LOGDEBUG, "last param: %s", buf->params[ii]);
	}
	return (0);
}

struct s_client_msg	*parse_input(char *input)
{
	struct s_client_msg	*msg;

	if (!input)
		return (0);
	msg = ft_memalloc(sizeof(*msg));
	if (!msg)
		return (0);
	msg->cmd = CMD_NONE;
	if (input[0] == '/')
	{
		input += 1;
		set_command(&input, msg);
		if (*input == ' ')
			input++;
		set_params(input, msg);
	}
	return (msg);
}

char				*format_message(struct s_client_msg *msg)
{
	char	*strmsg;

	strmsg = ft_strnew(512);
	if (msg->cmd == CMD_NICK)
	{
		ft_strlcat(strmsg, "NICK ", 512);
		ft_strlcat(strmsg, msg->params[0], 512);
	}
	else if (msg->cmd == CMD_MSG)
	{
		ft_strlcat(strmsg, "PRIVMSG ", 512);
		ft_strlcat(strmsg, msg->params[0], 512);
		ft_strlcat(strmsg, msg->params[1], 512);
	}
	else if (msg->cmd == CMD_JOIN)
	{
		ft_strlcat(strmsg, "JOIN ", 512);
		ft_strlcat(strmsg, msg->params[0], 512);
	}
	ft_strlcat(strmsg, CRLF, 512);
	return (strmsg);
}
