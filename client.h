#ifndef CLIENT_H
# define CLIENT_H

# include <string.h>
# include <libft.h>
# include <sys/types.h>
# include "parser/parser.h"
# include "channels.h"

# define CRLF	"\x0d\x0a"

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
	CMD_USER,
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

struct s_client
{
	int					is_running;
	int					servsock;
	t_list				*msgs;
	fd_set				readfds;
	fd_set				writefds;
	struct s_channels	channels;

	/*
	** Methods
	*/
	int (*run)(struct s_client *const);
	int (*exec_cmd)(struct s_client *const , struct s_client_msg const *const);
	int (*queuemsg)(struct s_client *const, char *msg);
	int	(*sendmsgs)(struct s_client *const);
	int	(*event)(struct s_client *const, char const *const event);

	/*
	** IRC Commands
	*/
	int (*connect)(struct s_client *const, struct s_client_msg const *const);
	int (*message)(struct s_client *const, struct s_client_msg const *const);
	int	(*nick)(struct s_client *const, struct s_client_msg const *const);
	int	(*join)(struct s_client *const, struct s_client_msg const *const);
	int	(*user)(struct s_client *const, struct s_client_msg const *const);

	int	(*eventjoin)(struct s_client *const, struct s_message const *const);
	int	(*eventprivmsg)(struct s_client *const, struct s_message const *const);

	int	(*rpl_welcome)(struct s_client *const, struct s_message const *const);
};

struct s_tuple_cmds
{
	enum e_client_cmd cmd;
	int (*f)(struct s_client *const, struct s_client_msg const *const);
};

struct s_event_list
{
	char	*s;
	int		(*f)(struct s_client *const, struct s_message const *const);
};

#endif
