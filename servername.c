#include "irc.h"

struct s_servername	*servername(char *input)
{
	struct s_servername	*s;

	s = ft_memalloc(sizeof(*s));
	if (s)
	{
		s->hostname = hostname(s);
		s->len += s->hostname->len;
	}
	return (s);
}

void	servername_del(struct s_servername **sn)
{
	hostname_del((*sn)->hostname);
	free(*sn);
	*sn = NULL;
}
