#include "cbuf.h"
#include <stdlib.h>

t_cbuf_handle	cbuf_init(uint8_t *buffer, size_t size)
{
	t_cbuf_handle	cbuf;

	cbuf = malloc(sizeof(*cbuf));
	cbuf->buffer = buffer;
	cbuf->max = size;
	cbuf_reset(cbuf);
	return (cbuf);
}
