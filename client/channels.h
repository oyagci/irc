#ifndef CHANNELS_H
# define CHANNELS_H

# include <libft.h>
# include "logger.h"

struct s_chan
{
	char	name[50];
	t_list	*clients;
};

struct s_channels
{
	t_list	*list;

	/*
	** Methods
	*/
	struct s_chan	*(*get)(struct s_channels *const, char const *const);
	struct s_chan	*(*create)(char const *const name);
	int				(*add)(struct s_channels *const, struct s_chan *);

	int				(*addnick)(struct s_channels *const,
							   char const *const nick,
							   char const *const channel);
	int				(*rmnick)(struct s_channels *const,
							  char const *const nick,
							  char const *const channel);
};

void	channels_init(struct s_channels *ptr);
int		channels_addclient(struct s_channels *self, char const *const nick,
						   char const *const chan);

#endif
