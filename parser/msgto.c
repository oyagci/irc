#include "../libft/includes/libft.h"
#include "../irc.h"

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
		if (*input == '#')
		{
			ret = chanstr(input + 1, &buf);
			if (ret > 0)
			{
				elem = ft_lstnew(0, 0);
				elem->content = buf;
				ft_lstpush(listbuf, elem);
			}
		}
		else if ((ret = nickname(input, &buf)) > 0)
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
