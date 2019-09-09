#include "cbuf.h"

static void	retreat_pointer(t_cbuf_handle cbuf)
{
	cbuf->full = 0;
	cbuf->tail = (cbuf->tail + 1) % cbuf->max;
}

int			cbuf_get(t_cbuf_handle cbuf, uint8_t *data)
{
	int	ret;

	ret = -1;
	if (!cbuf_empty(cbuf))
	{
		*data = cbuf->buffer[cbuf->tail];
		retreat_pointer(cbuf);
		ret = 0;
	}
	return (ret);
}
