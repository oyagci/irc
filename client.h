#ifndef CLIENT_H
# define CLIENT_H

# include <string.h>

enum e_client_cmd
{
	CMD_NONE,
	CMD_CONNECT,
	CMD_DISCONNECT,
	CMD_JOIN,
	CMD_LEAVE,
	CMD_MSG,
	CMD_NICK,
	CMD_WHO,
};

struct s_client_cmds
{
	char const *const	str;
	enum e_client_cmd	cmd;
	short				nparam;
};

struct s_cmd_params
{
	enum e_client_cmd	cmd;
	short				nparam;
};

struct s_client_msg
{
	enum e_client_cmd	cmd;
	char				**params;
	size_t				nparam;
};

struct s_client_msg	*parse_input(char *input);
char				*format_message(struct s_client_msg *msg);

#endif
