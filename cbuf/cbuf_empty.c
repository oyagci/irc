#include "cbuf.h"

int	cbuf_empty(t_cbuf_handle cbuf)
{
	return (!cbuf->full && cbuf->tail == cbuf->head);
}
