#include "nick.h"
#include "libft.h"
#include <stdlib.h>
#include "server.h"

int		nickavail(t_nicktable *nicks, char const *nick)
{
	size_t	i;

	i = 0;
	while (i < nicks->size)
	{
		if (ft_strequ(nicks->table[i], nick))
			return (0);
		i++;
	}
	return (1);
}

int		nickadd(t_nicktable *nicks, char const *nick)
{
	if (!nickavail(nicks, nick))
		return (0);
	if (nicks->size == nicks->capacity)
	{
		nicks->table = realloc(nicks->table, sizeof(char *) * (nicks->capacity + 10));
		nicks->capacity += 10;
	}
	nicks->table[nicks->size] = ft_strdup(nick);
	nicks->size += 1;
	return (1);
}

int		nickinit(t_nicktable *nicks)
{
	int	ret;

	ret = 0;
	nicks->size = 0;
	nicks->capacity = 10;
	nicks->table = ft_memalloc(sizeof(*nicks->table) * nicks->capacity);
	if (!nicks->table)
		ret = -1;
	return (ret);
}

void	nickremove(t_nicktable *nicks, char *name)
{
	size_t	i;

	i = 0;
	while (i < nicks->size)
	{
		if (ft_strnequ(name, nicks->table[i], NICK_SIZE))
			nicks->table[i] = 0;
		i += 1;
	}
}
