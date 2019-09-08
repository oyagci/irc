#include "client.h"
#include <stdio.h>
#include "logger.h"
#include "libft.h"
#include <stdlib.h>

enum e_client_cmd	set_command(char const **inputp, struct s_client_msg *buf)
{
	const struct s_client_cmds	cmds[] = {
		{ .str = "connect", .cmd = CMD_CONNECT, .nparam = 2 },
		{ .str = "nick", .cmd = CMD_NICK, .nparam = 1 },
		{ .str = "join", .cmd = CMD_JOIN, .nparam = 1 },
		{ .str = "leave", .cmd = CMD_LEAVE, .nparam = 1 },
		{ .str = "msg", .cmd = CMD_MSG, .nparam = 2 },
		{ .str = "quit", .cmd = CMD_QUIT, .nparam = 1 },
		{ .str = "who", .cmd = CMD_WHO, .nparam = 0 },
		{ .str = "disconnect", .cmd = CMD_QUIT, .nparam = 0 }
	};
	char const					*input;
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

size_t				set_params(char const *input, struct s_client_msg *buf)
{
	int		start;
	int		end;
	size_t	ii;
	char	tmp[255];

	if (*input == 0)
		return (0);
	start = 0;
	end = 0;
	ii = 0;
	if (buf->nparam > 0)
	{
		while (input[end] != '\0' && ii < buf->nparam - 1)
		{
			end = start;
			while (input[end] != ' ' && input[end] != '\0')
				end++;
			ft_memset(tmp, 0, sizeof(tmp));
			ft_strncpy(tmp, input + start, end - start);
			ft_strlcpy(buf->params[ii], tmp, 255);
			if (input[end] == ' ')
				end++;
			start = end;
			ii += 1;
		}
		if (input[start] != 0) {
			ft_strlcpy(buf->params[ii], input + start, 255);
			ii += 1;
		}
	}
	return (ii);
}

struct s_client_msg	*parse_input(struct s_client *const self, char const *input)
{
	struct s_client_msg	*msg;

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
		if (set_params(input, msg) < msg->nparam)
		{
			printf("Not enough parameters given (expected %ld)\n", msg->nparam);
			free(msg);
			msg = NULL;
		}
	}
	else if (self->channel)
	{
		msg->cmd = CMD_MSG;
		msg->nparam = 2;
		ft_strlcpy(msg->params[0], self->channel->name, 255);
		ft_strlcpy(msg->params[1], input, 255);
	}
	else
	{
		printf(" - Not in a channel (/join <channel>)\n");
		free(msg);
		msg = NULL;
	}
	return (msg);
}
