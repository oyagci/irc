#include "client.h"

int		client_event(struct s_client *self, char const *const data)
{
	struct s_event_list events[] = {
		{ .s = "JOIN", .f = self->eventjoin },
		{ .s = "PRIVMSG", .f = self->eventprivmsg },
		{ .s = "PART", .f = self->eventpart },
		{ .s = "PING", .f = self->eventping },
		{ .s = "001", .f = self->rpl_welcome },
		{ .s = "372", .f = self->eventmotd },
		{ .s = "375", .f = self->eventmotd },
	};
	size_t				ii;

	struct s_message	*msg;

	msg = message(data);
	if (!msg)
	{
		printf("Invalid command\n");
		return (0);
	}
	ii = 0;
	while (ii < sizeof(events) / sizeof(*events))
	{
		if (ft_strequ(events[ii].s, msg->cmd->data))
			events[ii].f(self, msg);
		ii += 1;
	}
	return (0);
}