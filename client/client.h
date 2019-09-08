#ifndef CLIENT_H
# define CLIENT_H

# include <string.h>
# include <libft.h>
# include <sys/types.h>
# include "parser/parser.h"
# include "channels.h"
# include <cbuf.h>

# define CRLF	"\x0d\x0a"
# define SNICK	9
# define NPARAM	15
# define SPARAM	255

# define BOLDRED	"\033[1;32m"
# define UNDERLINE	"\033[0;0;4m"
# define RESET		"\033[0;0m"

enum e_client_cmd
{
	CMD_NONE,
	CMD_CONNECT,
	CMD_QUIT,
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
	char 				params[NPARAM][SPARAM];
	size_t				nparam;
};

struct s_client
{
	short				is_running;
	short				is_registered;
	int					servsock;
	t_list				*msgs;
	fd_set				readfds;
	fd_set				writefds;
	struct s_channels	channels;
	struct s_chan		*channel;
	char				nickname[SNICK];
	t_cbuf_handle		cbuf;
	uint8_t				raw_buffer[2048];

	/*
	** Methods
	*/
	int (*run)(struct s_client *const);
	int (*exec_cmd)(struct s_client *const , struct s_client_msg const *const);
	int (*queuemsg)(struct s_client *const, char *msg);
	int	(*sendmsgs)(struct s_client *const);
	int	(*event)(struct s_client *const, char const *const event);
	struct s_client_msg	*(*parse_input)(struct s_client *const,
		char const *input);

	/*
	** IRC Commands
	*/
	int (*connect)(struct s_client *const, struct s_client_msg const *const);
	int (*message)(struct s_client *const, struct s_client_msg const *const);
	int	(*nick)(struct s_client *const, struct s_client_msg const *const);
	int	(*join)(struct s_client *const, struct s_client_msg const *const);
	int	(*user)(struct s_client *const, struct s_client_msg const *const);
	int	(*leave)(struct s_client *const, struct s_client_msg const *const);
	int	(*quit)(struct s_client *const, struct s_client_msg const *const);
	int	(*who)(struct s_client *const, struct s_client_msg const *const);

	int	(*eventjoin)(struct s_client *const, struct s_message const *const);
	int	(*eventprivmsg)(struct s_client *const, struct s_message const *const);
	int	(*eventpart)(struct s_client *const, struct s_message const *const);
	int	(*eventping)(struct s_client *const, struct s_message const *const);
	int	(*eventmotd)(struct s_client *const, struct s_message const *const);

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

struct s_client_msg	*parse_input(struct s_client *const self,
									char const *input);
char				*format_message(struct s_client_msg *msg);

int					client_run(struct s_client *const self);

int		quit(struct s_client *const self, struct s_client_msg const *const msg);
int		eventmotd(struct s_client *const self,
					struct s_message const *const cmd);
int		eventpart(struct s_client *const self,
					struct s_message const *const cmd);
int		eventjoin(struct s_client *const self, struct s_message const *const m);
int		eventprivmsg(struct s_client *const self,
						struct s_message const *const m);
int		eventping(struct s_client *const self,
					struct s_message const *const msg);

int		client_init(struct s_client *const self);
int		client_sendmsgs(struct s_client *const self);
int		client_queuemsg(struct s_client *const self, char *msg);
int		client_message(struct s_client *const self,
					   struct s_client_msg const *const cmd);

int		client_event(struct s_client *const self, char const *const data);
int		client_leave(struct s_client *const self,
					 struct s_client_msg const *const cmd);
int		client_user(struct s_client *const self,
					struct s_client_msg const *const cmd);
int		client_join(struct s_client *const self,
					struct s_client_msg const *const cmd);
int		client_execute_command(struct s_client *const self,
							   struct s_client_msg const *const cmd);
int		client_who(struct s_client *const self,
				   struct s_client_msg const *const cmd);
int		client_nick(struct s_client *const self,
					struct s_client_msg const *const cmd);
int		client_connect(struct s_client *const self,
					   struct s_client_msg const *const cmd);
int		rpl_welcome(struct s_client *const self,
					struct s_message const *const msg);

#endif
