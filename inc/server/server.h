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
# include "libft.h"
# include "parser/parser.h"
# include "reply_codes.h"
# include "nick.h"
# include <cbuf.h>

# define COMMAND_LENGTH		512
# define CRLF				"\x0d\x0a"
# define CLIENT_BUFFER_SIZE	COMMAND_LENGTH
# define MAX_CONN			1024
# define NICK_SIZE			9
# define NCLIENTS			10

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

typedef struct	s_client
{
	int						fd;
	int						lastret;
	struct s_client_buffer	buffer;
	struct timespec			ping;
	struct timespec			timeout;
	t_cbuf_handle			cbuf;
	uint8_t					raw_buffer[2048];
	int						nmsg;
	int						ncmds;

	short					is_connected;
	short					is_registered;

	enum e_umode			umode;
	char					nickname[NICK_SIZE + 1];
	char					*username;
	char					*realname;

	short					should_be_disconnected;

	struct s_server			*server;
}				t_client;

struct s_server
{
	short		is_running;
	fd_set		readfds;
	fd_set		writefds;
	int			sockfd;
	t_list		*channels;

	t_client	clients[NCLIENTS];
	size_t		nclients;

	t_list		*msgqueue;
	t_nicktable	nicks;
};

struct s_server_msg
{
	char	dest[NICK_SIZE];
	char	msg[512];
	size_t	len;
};

int server_init(struct s_server *server, unsigned int port);
int execute_command(struct s_server *self, struct s_client *c, char const *const cmd);
int read_client_command(struct s_server *const self);
int server_read_clients_command( struct s_server *const server);
int accept_new_clients(struct s_server *server);
int reply_client(struct s_client *c, int retcode);
int docommands(struct s_server *const self);

/*
** IRC server commands
*/
int irc_pass(struct s_client *c, struct s_params *p);
int irc_nick(struct s_client *c, struct s_params *p);
int irc_user(struct s_client *c, struct s_params *p);
int irc_oper(struct s_client *c, struct s_params *p);
int irc_join(struct s_client *c, struct s_params *p);
int irc_who(struct s_client *c, struct s_params *p);
int irc_part(struct s_client *c, struct s_params *p);
int irc_privmsg(struct s_client *c, struct s_params *p);
int irc_ping(struct s_client *c, struct s_params *p);
int irc_pong(struct s_client *c, struct s_params *p);
int irc_quit(struct s_client *c, struct s_params *p);

int set_usermode(struct s_client *c, int mode);
int set_realname(struct s_client *c, char *rn);

int run(struct s_server *);

int send_queued_replies(struct s_server *const server);
int queue_reply(struct s_server *server, struct s_client *const dest, char const *reply);
int queue_code_reply(struct s_server *server, struct s_client *const dest, int reply_code);

int add_to_chan(struct s_server *server, struct s_client *client, char const *const channame);

int server_tell_new_client(struct s_server *server, struct s_client *client, struct s_channel *chan);

struct s_channel *get_channel(struct s_server *server, char const *name);

struct s_client *get_client(struct s_server *self, char const *const nickname);

void      del_client(struct s_server *self, struct s_client *c);

int       server_send_formated_message_to( struct s_server *server, char const *recipient, char *msg);

void      server_msg_del(void *msgp, size_t size);

int       channel_add_client(struct s_channel *channel, struct s_client *client);

int       rm_from_chan(char *nick, struct s_channel *chan);

int       notifypart(struct s_server *s, struct s_channel *chan, char const *const nick);

struct s_channel *create_channel(struct s_server *server, char const *name, int mode);

int pinginactive(struct s_server *self);

int quit(struct s_server *self, struct s_client *client, char const *const msg);

int update_clients(struct s_server *self);

/*
 * Replies
 */
int rpl_welcome(char *buf, size_t buflen, struct s_client const *c);
int err_unknowncmd(char *buf, size_t buflen, struct s_client const *c);
int err_nickinuse(char *buf, size_t buflen, struct s_client const *c);
int err_erroneusnick(char *buf, size_t buflen, struct s_client const *c);

#endif
