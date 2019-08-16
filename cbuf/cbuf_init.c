#include "cbuf.h"

void	cbuf_init(t_cbuf_handle cbuf, uint8_t *buffer, size_t size)
{
	cbuf->buffer = buffer;
	cbuf->max = size;
	cbuf_reset(cbuf);
}
