#ifndef CHANNELS_H
# define CHANNELS_H

# include <libft.h>

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
	int				(*create)(struct s_channels *const, char const *const name);
};

void				channels_init(struct s_channels *ptr);

#endif
