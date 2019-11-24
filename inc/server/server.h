/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   irc.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oyagci <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/05 13:51:41 by oyagci            #+#    #+#             */
/*   Updated: 2019/11/22 13:16:05 by oyagci           ###   ########.fr       */
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

typedef struct	s_channel
{
	char	name[50];
	char	*topic;
	int		mode;
	t_list	*clients;
}				t_channel;

typedef struct	s_client_buffer {
	char	data[CLIENT_BUFFER_SIZE];
	int		len;
	short	is_complete;
}				t_client_buffer;

typedef struct	s_client
{
	int				fd;
	t_cbuf_handle	cbuf;
	uint8_t			raw_buffer[2048];
	int				nmsg;
	int				ncmds;

	short			is_connected;
	short			is_registered;

	char			nickname[NICK_SIZE + 1];

	short			should_be_disconnected;

	t_cbuf_handle	reply;
	uint8_t			replybuf[2048];

	struct s_server	*server;
}				t_client;

typedef struct	s_server
{
	short		is_running;
	fd_set		readfds;
	fd_set		writefds;
	int			sockfd;
	t_list		*channels;

	t_client	clients[NCLIENTS];

	t_list		*msgqueue;
	t_nicktable	nicks;
}				t_server;

int				server_init(struct s_server *server, unsigned int port);
int				execute_command(struct s_server *self, struct s_client *c,
					char const *const cmd);
int				read_client_command(struct s_server *const self);
int				server_read_clients_command( struct s_server *const server);
int				accept_new_clients(struct s_server *server);
int				reply_client(struct s_client *c, int retcode);
int				docommands(struct s_server *const self);

/*
** IRC server commands
*/
int				irc_pass(struct s_client *c, struct s_params *p);
int				irc_nick(struct s_client *c, struct s_params *p);
int				irc_user(struct s_client *c, struct s_params *p);
int				irc_oper(struct s_client *c, struct s_params *p);
int				irc_join(struct s_client *c, struct s_params *p);
int				irc_who(struct s_client *c, struct s_params *p);
int				irc_part(struct s_client *c, struct s_params *p);
int				irc_privmsg(struct s_client *c, struct s_params *p);
int				irc_ping(struct s_client *c, struct s_params *p);
int				irc_pong(struct s_client *c, struct s_params *p);
int				irc_quit(struct s_client *c, struct s_params *p);

int				validate_nickname(char const *input);

int				run(struct s_server *);
int				send_queued_replies(struct s_server *const server);
int				queue_reply(t_server *server, t_client *const dest,
					char const *reply);
int				queue_code_reply(t_server *server, t_client *const dest,
					int reply_code);
int				add_to_chan(struct s_server *server, t_client *client,
					char const *const channame);
int				server_tell_new_client(t_server *server, t_client *client,
					t_channel *chan);
t_channel		*get_channel(struct s_server *server, char const *name);
t_client		*get_client(struct s_server *self, char const *const nickname);
void     		del_client(struct s_server *self, struct s_client *c);
int				server_send_formated_message_to(t_server *server,
					char const *recipient, char *msg);
void			server_msg_del(void *msgp, size_t size);
int				channel_add_client(t_channel *channel, t_client *client);
int				rm_from_chan(char *nick, struct s_channel *chan);
int				notifypart(t_server *s, t_channel *chan,
					char const *const nick);
t_channel		*create_channel(t_server *server, char const *name, int mode);
int				pinginactive(t_server *self);
int				quit(t_server *self, t_client *client, char const *const msg);
int				update_clients(t_server *self);

/*
 * Replies
 */
int				rpl_welcome(char *buf, size_t buflen, t_client const *c);
int				err_unknowncmd(char *buf, size_t buflen, t_client const *c);
int				err_nickinuse(char *buf, size_t buflen, t_client const *c);
int				err_erroneusnick(char *buf, size_t buflen, t_client const *c);

typedef struct	s_tuple_cmds {
	char const	*name;
	int			(*f)(t_client *c, struct s_params *p);
}				t_tuple_cmds;

typedef struct	s_tuple_reply {
	int	num;
	int	(*f)(char *, size_t, t_client const *);
}				t_tuple_reply;

#endif
