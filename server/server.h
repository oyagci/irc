/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   irc.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oyagci <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/05 13:51:41 by oyagci            #+#    #+#             */
/*   Updated: 2019/08/09 16:08:44 by oyagci           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_H
# define SERVER_H

# include <string.h>
# include <sys/select.h>
# include "libft/includes/libft.h"
# include "parser/parser.h"
# include "reply_codes.h"

# define COMMAND_LENGTH		512
# define CRLF				"\x0d\x0a"
# define CLIENT_BUFFER_SIZE	COMMAND_LENGTH
# define MAX_CONN			1024
# define NICK_SIZE			9

/*
** Connection password
** Must be either `0' (zero) or
** a valid c-string password
*/
# define SERVER_PASS			0

struct	s_channel
{
	char	name[50];
	char	*topic;
	int		mode;
	t_list	*clients;
};

struct	s_client_buffer {
	char	data[CLIENT_BUFFER_SIZE];
	int		len;
	short	is_complete;
};

enum	e_umode
{
	UMODE_WALLOPS = 1 << 2,
	UMODE_INVISIBLE = 1 << 3,
};

struct	s_client
{
	int						fd;
	int						lastret;
	struct s_client_buffer	buffer;

	short					is_connected;
	short					is_registered;

	enum e_umode			umode;
	char					nickname[NICK_SIZE + 1];
	char					*username;
	char					*realname;

	struct s_server			*server;
};

struct	s_server
{
	short				is_running;
	fd_set				readfds;
	fd_set				writefds;
	int					sockfd;
	t_list				*channels;
	t_list				*clients;
	t_list				*msgqueue;
	int					(*run)(struct s_server *const self);
	int					(*set_fds)(struct s_server *const self);
	int					(*accept)(struct s_server *const self);
	int					(*read)(struct s_server *const self);
	int					(*send)(struct s_server *const self);
	int					(*exec_cmd)(struct s_client *c);
	int					(*queuecode)(struct s_server *self,
							struct s_client const *const dest, int code);
	int					(*queuenotif)(struct s_server *self,
							struct s_client const *const dest, char *msg);
	int					(*rm_from_chan)(char *const nick,
							struct s_channel *chan);
	struct s_channel	*(*get_channel)(struct s_server *self,
							char const *const name);
	int					(*notifypart)(struct s_server *self,
							struct s_channel *chan,
							char const *const nick);
};

struct	s_server_msg
{
	struct s_client const	*dest;
	char					msg[512];
	size_t					len;
};

int					server_init(struct s_server *server, unsigned int port);
int					set_fds(struct s_server *server);
int					execute_command(struct s_client *c);
int					read_clients_command(struct s_server *const self);
int					server_read_clients_command(struct s_server *const server);
int					accept_new_clients(struct s_server *server);
int					reply_client(struct s_client *c, int retcode);

/*
** IRC server commands
*/

/*
** Tuple-like structure
*/
typedef int	(*t_irc_func)(struct s_client *c, char **params, int nparams);
struct	s_irc_cmds
{
	char const	*name;
	t_irc_func	f;
};

int					irc_pass(struct s_client *, char **params, int nparam);
int					irc_nick(struct s_client *, char **params, int nparam);
int					irc_user(struct s_client *, char **params, int nparam);
int					irc_oper(struct s_client *, char **params, int nparam);
int					irc_join(struct s_client *, char **params, int nparam);
int					irc_who(struct s_client *client, char **params, int nparam);
int					irc_part(struct s_client *client, char **params,
						int nparam);
int					irc_privmsg(struct s_client *client, char **params,
						int nparams);

/* TODO: Handle nick collision */
int					nickavail(char *nick);
int					nickadd(char *nick);

int					set_usermode(struct s_client *c, int mode);
int					set_realname(struct s_client *c, char *rn);

int					loop(struct s_server *server);

int					send_queued_replies(struct s_server *const server);
int					queue_reply(struct s_server *server,
						struct s_client const *const dest,
						char *reply);
int					queue_code_reply(struct s_server *server,
						struct s_client const *const dest, int reply_code);

int					server_add_to_chan(struct s_server *server,
						struct s_client *client, char const *const channame);
int					server_tell_new_client(struct s_server *server,
						struct s_client *client, struct s_channel *chan);
struct s_channel	*get_channel(struct s_server *server, char const *name);
int					server_send_formated_message_to(struct s_server *server,
						char const *recipient, char *msg);
void				server_msg_del(void *msgp, size_t size);

int					channel_add_client(struct s_channel *channel,
						struct s_client *client);
int					channel_rm_nick(struct s_channel *const channel,
						char const *const nick);
int					server_rm_nick(struct s_server *server,
						char const *const nick, char const *const chan);

int					server_rm_from_chan(char *nick, struct s_channel *chan);

int					notifypart(struct s_server *s,
						struct s_channel *chan, char const *const nick);

char				*irc_repcode_itoa(unsigned int n);

#endif