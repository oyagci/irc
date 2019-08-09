/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   irc.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oyagci <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/05 13:51:41 by oyagci            #+#    #+#             */
/*   Updated: 2019/08/09 13:27:54 by oyagci           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef IRC_H
# define IRC_H

#include <string.h>
#include <sys/select.h>
#include "libft.h"

# define COMMAND_LENGTH		512
# define CRLF				"\x0d\x0a"
# define CLIENT_BUFFER_SIZE	COMMAND_LENGTH
# define MAX_CONN			1024
# define NICK_SIZE			9

# define ERR_UNKNOWNCOMMAND		421
# define ERR_NONICKNAMEGIVEN	431
# define ERR_ERRONEUSNICKNAME	432
# define ERR_NICKNAMEINUSE		433
# define ERR_NEEDMOREPARAM		461
# define ERR_ALREADYREGISTRED	462
# define ERR_PASSWDMISMATCH		464

# define RPL_WELCOME			1

/*
** Connection password
** Must be either `0' (zero) or
** a valid c-string password
*/
# define SERVER_PASS			0

struct s_channel
{
	char	name[50];
	char	*topic;
	int		mode;
	t_list	*clients;
};

struct s_client_buffer {
	char	data[CLIENT_BUFFER_SIZE];
	int		len;
	short	is_complete;
};

enum e_umode
{
	UMODE_WALLOPS = 1 << 2,
	UMODE_INVISIBLE = 1 << 3,
};

struct s_client
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

struct s_server
{
	fd_set	readfds;
	fd_set	writefds;
	int		sockfd;
	t_list	*channels;
	t_list	*clients;
	t_list	*msgqueue;
};

struct s_server_msg
{
	struct s_client const	*dest;
	char					*msg;
	size_t					len;
};

/*
** Helper structs for parsing
*/

struct s_message
{
	struct s_prefix		*prefix;
	struct s_command	*cmd;
	struct s_params		*params;
	size_t				len;
};

struct s_prefix
{
	char				*data;
	size_t				len;
};

struct s_command
{
	char	*data;
	size_t	len;
};

struct s_crlf
{
};

/*
** RFC2812 tells us that the maximum number of parameters
** per message is 15 (fifteen)
*/
struct s_params
{
	char	*param[15];
	size_t	len;
};

/*
** End of helper structs
*/

struct s_message	*message(char const *input);
struct s_prefix		*prefix(char const *input);
struct s_command	*command(char const *input);
struct s_crlf		*crlf(char const *input);
struct s_params		*params(char const *input);
char				*host(char const *input);
int					user(unsigned char const *input, char **buffer);
int					nickname(char const *input, char **buffer);
t_list				*channels(char *input);

void				prefix_del(struct s_prefix **p);
void				command_del(struct s_command **cmd);
void				message_del(struct s_message **msg);

int					set_fds(int max_sd, t_list *clients, fd_set *readfds, fd_set *writefds);
int					execute_command(struct s_client *c);
int					read_client_command(int cfd, struct s_client_buffer *buffer);
int					server_read_clients_command(struct s_server const *const server, t_list *clients);
int					server_accept_new_clients(struct s_server *server);
int					reply_client(struct s_client *c, int retcode);

/*
** IRC server commands
*/

/*
** Tuple-like structure
*/
typedef int (*t_irc_func)(struct s_client *c, char **params, int nparams);
struct s_irc_cmds
{
	char const	*name;
	t_irc_func	f;
};

int		irc_pass(struct s_client *, char **, int);
int		irc_nick(struct s_client *, char **, int);
int		irc_user(struct s_client *, char **, int);
int		irc_oper(struct s_client *, char **, int);
int		irc_join(struct s_client *, char **, int);

int		nickavail(char *nick);
int		nickadd(char *nick);

int		set_usermode(struct s_client *c, int mode);
int		set_realname(struct s_client *c, char *rn);

int		server_send_queued_replies(struct s_server *const server);
int		server_queue_reply(struct s_server *server,
	struct s_client const *const dest, char *reply);
int			server_queue_code_reply(struct s_server *server,
	struct s_client const *const dest, int reply_code);
int		server_add_to_chan(struct s_server *server, struct s_client *client,
	char const *const channame);
int	server_tell_new_client(struct s_server *server, struct s_client *client,
		struct s_channel *chan);

void	server_msg_del(void *msgp, size_t size);

int		channel_add_client(struct s_channel *channel, struct s_client *client);

#endif
