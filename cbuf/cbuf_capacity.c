#include "cbuf.h"

size_t	cbuf_capacity(t_cbuf_handle cbuf)
{
	return (cbuf->max);
}
