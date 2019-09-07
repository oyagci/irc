#ifndef NICK_H
# define NICK_H

# include <stddef.h>

typedef struct	s_nicktable {
	char	**table;
	size_t	size;
	size_t	capacity;
}				t_nicktable;

int	nickinit(t_nicktable *nicks);
int	nickavail(t_nicktable *nicks, char const *nick);
int	nickadd(t_nicktable *nicks, char const *nick);

#endif
