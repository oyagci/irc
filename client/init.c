#include "client.h"
#include <stdlib.h>

void	init_events(struct s_client *self)
{
	self->eventping = &eventping;
	self->eventjoin = &eventjoin;
	self->eventprivmsg = &eventprivmsg;
	self->eventpart = &eventpart;
	self->eventmotd = &eventmotd;
	self->rpl_welcome = &rpl_welcome;
}

void	init_messages(struct s_client *self)
{
	self->connect = &client_connect;
	self->message = &client_message;
	self->nick = &client_nick;
	self->join = &client_join;
	self->user = &client_user;
	self->leave = &client_leave;
	self->who = &client_who;
	self->quit = &quit;
}

void	init_methods(struct s_client *self)
{
	self->run = &client_run;
	self->exec_cmd = &client_execute_command;
	self->queuemsg = &client_queuemsg;
	self->sendmsgs = &client_sendmsgs;
	self->event = &client_event;
	self->parse_input = &parse_input;
}

void	client_init(struct s_client *self)
{
	if (!self)
		return ;
	self->servsock = 0;
	self->channel = 0;
	self->msgs = NULL;
	ft_memset(self->nickname, 0, 9);
	channels_init(&self->channels);
	init_methods(self);
	init_events(self);
	init_messages(self);
	ft_bzero(self->raw_buffer, 2048);
	self->cbuf = cbuf_init(self->raw_buffer, 2048);
}
