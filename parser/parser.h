#ifndef PARSER_H
# define PARSER_H

# include <string.h>
# include <libft.h>

/*
** Helper structs for parsing
*/

struct s_message
{
	struct s_prefix		*prefix; struct s_command	*cmd;
	struct s_params		*params;
	struct s_crlf		*crlf;
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
	size_t	nparam;
};

struct s_message	*message(char const *input);
struct s_prefix		*prefix(char const *input);
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

void				prefix_del(struct s_prefix **p);
void				command_del(struct s_command **cmd);
void				message_del(struct s_message **msg);
void				msgto_del(t_list **lp);
void				crlf_del(struct s_crlf **p);
void				channels_del(t_list **chansp);
void				params_del(struct s_params **paramsp);

#endif
