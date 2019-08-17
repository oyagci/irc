#include "../libft/includes/libft.h"
#include "parser.h"
#include "../logger.h"
#include <stdlib.h>

/*
** msgtarget  =  msgto *( "," msgto )
** msgto      =  channel / ( user [ "%" host ] "@" servername )
** msgto      =/ ( user "%" host ) / targetmask
** msgto      =/ nickname / ( nickname "!" user "@" host )
*/
int			msgto(char *input, t_list **listbuf)
{
	char	*buf;
	int		ret;
	t_list	*elem;

	*listbuf = NULL;
	while (42)
	{
		if ((ret = chanstr(input, &buf)) > 0 ||
			(ret = nickname(input, &buf)) > 0)
		{
			elem = ft_lstnew(0, 0);
			elem->content = buf;
			ft_lstpush(listbuf, elem);
		}
		input += ret;
		if (*input == ',')
			input += 1;
		else
			break ;
	}
	return (0);
}

void	msgto_del(t_list **lp)
{
	t_list	*l;
	t_list	*next;

	l = *lp;
	next = NULL;
	while (l)
	{
		next = l->next;
		free(l->content);
		free(l);
		l = next;
	}
}
