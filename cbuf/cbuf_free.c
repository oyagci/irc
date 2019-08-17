#include "cbuf.h"
#include <stdlib.h>

void	cbuf_free(t_cbuf_handle cbuf)
{
	free(cbuf);
}
