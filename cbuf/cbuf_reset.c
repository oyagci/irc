#include "cbuf.h"

void	cbuf_reset(t_cbuf_handle cbuf)
{
	cbuf->head = 0;
	cbuf->tail = 0;
	cbuf->full = 0;
}
