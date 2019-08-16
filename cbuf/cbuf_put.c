#include "cbuf.h"

static void	advance_pointer(t_cbuf_handle cbuf)
{
	if (cbuf->full)
		cbuf->tail = (cbuf->tail + 1) % cbuf->max;
	cbuf->head = (cbuf->head + 1) % cbuf->max;
	cbuf->full = cbuf->head == cbuf->tail;
}

void		cbuf_put(t_cbuf_handle cbuf, uint8_t data)
{
	cbuf->buffer[cbuf->head] = data;
	advance_pointer(cbuf);
}

int			cbuf_put2(t_cbuf_handle cbuf, uint8_t data)
{
	int	ret;

	ret = -1;
	if (!cbuf_full(cbuf))
	{
		cbuf->buffer[cbuf->head] = data;
		advance_pointer(cbuf);
		ret = 0;
	}
	return (ret);
}
