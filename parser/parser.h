/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oyagci <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/09/09 14:56:20 by oyagci            #+#    #+#             */
/*   Updated: 2019/09/09 14:56:25 by oyagci           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSER_H
# define PARSER_H

# include <string.h>
# include <libft.h>

/*
** Helper structs for parsing
*/

struct				s_prefix
{
	char				data[255];
	size_t				len;
};

struct				s_message
{
	struct s_prefix		prefix;
	struct s_command	*cmd;
	struct s_params		*params;
	struct s_crlf		*crlf;
	size_t				len;
};

struct				s_command
{
	char	*data;
	size_t	len;
};

struct				s_crlf
{
	char c;
};

struct				s_params
{
	char	*param[15];
	size_t	len;
	size_t	nparam;
};

struct s_message	*message(char const *input);
int					prefix(struct s_prefix *p, char const *input);
struct s_command	*command(char const *input);
struct s_crlf		*crlf(char const *input);
struct s_params		*params(char const *input);
char				*host(char const *input);
int					user(unsigned char const *input, char **buffer);
int					nickname(char const *input, char **buffer);
t_list				*channels(char *input);
char				special(char input);
char				letter(char input);
char				digit(char input);
int					chanstr(char const *inputc, char **buffer);
int					msgto(char *input, t_list **listbuf);

void				command_del(struct s_command **cmd);
void				message_del(struct s_message **msg);
void				msgto_del(t_list **lp);
void				crlf_del(struct s_crlf **p);
void				channels_del(t_list **chansp);
void				params_del(struct s_params **paramsp);
int					is_nospcrlfcl(unsigned char c);

#endif
