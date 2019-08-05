/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   irc.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oyagci <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/05 13:51:41 by oyagci            #+#    #+#             */
/*   Updated: 2019/08/05 16:44:01 by oyagci           ###   ########.fr       */
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
# define MAX_CONN			10
# define NICK_SIZE			9

struct s_client_buffer {
	char	data[CLIENT_BUFFER_SIZE];
	int		len;
	short	is_complete;
};

struct s_client
{
	int						fd;
	struct s_client_buffer	buffer;
	char					nickname[NICK_SIZE];
	char					old_nickname[NICK_SIZE];
};

struct s_server
{
	fd_set	readfds;
	fd_set	writefds;
	int		sockfd;
};

/*
** Tuple-like structure
*/
typedef int (*t_irc_func)(char **params);
struct s_irc_cmds
{
	char const	*name;
	t_irc_func	f;
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

void				prefix_del(struct s_prefix **p);
void				command_del(struct s_command **cmd);
void				message_del(struct s_message **msg);

int					set_fds(int max_sd, t_list *clients, fd_set *readfds, fd_set *writefds);
int					execute_command(char *data);
int					read_client_command(int cfd, struct s_client_buffer *buffer);
int					handle_io_clients(struct s_server const *const s, t_list *clients);
int					handle_new_clients(int sockfd, t_list **clients, fd_set *readfds);

#endif
