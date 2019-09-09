/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   messages.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oyagci <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/09/09 13:04:49 by oyagci            #+#    #+#             */
/*   Updated: 2019/09/09 13:04:52 by oyagci           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "client.h"
#include <stdlib.h>
#include <sys/socket.h>

void	client_del_msg(void *content, size_t size)
{
	free(content);
	(void)size;
}

int		client_sendmsgs(struct s_client *const self)
{
	char	*str;
	int		len;
	int		ret;
	t_list	*next;

	if (self->msgs && FD_ISSET(self->servsock, &self->writefds))
	{
		str = self->msgs->content;
		len = ft_strlen(str);
		ret = send(self->servsock, str, ft_strlen(str), 0);
		if (ret == len)
		{
			next = self->msgs->next;
			ft_lstdelone(&self->msgs, &client_del_msg);
			self->msgs = next;
		}
		else if (ret > 0)
			ft_memmove(str, str + ret, ft_strlen(str + ret));
		else
		{
			LOG(LOGDEBUG, "%s: could not send message", __FUNCTION__);
			return (-1);
		}
	}
	return (0);
}

int		client_queuemsg(struct s_client *const self, char *msg)
{
	t_list	*elem;

	elem = ft_lstnew(0, 0);
	if (!elem)
		return (-1);
	elem->content = msg;
	elem->content_size = 0;
	ft_lstpush(&self->msgs, elem);
	return (0);
}

int		client_message(struct s_client *const self,
	struct s_client_msg const *const cmd)
{
	char	*msg;

	if (!self->servsock)
	{
		printf(" * Not connected to a server\n");
		return (0);
	}
	msg = ft_strnew(513);
	if (!msg)
		return (-1);
	ft_strlcat(msg, "PRIVMSG", 513);
	ft_strlcat(msg, " ", 513);
	ft_strlcat(msg, cmd->params[0], 513);
	ft_strlcat(msg, " :", 513);
	ft_strlcat(msg, cmd->params[1], 513);
	ft_strlcat(msg, CRLF, 513);
	return (self->queuemsg(self, msg));
}
