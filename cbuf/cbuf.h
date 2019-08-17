#ifndef CBUF_H
# define CBUF_H

# include <string.h>
# include <stdint.h>

struct	s_cbuf
{
	uint8_t	*buffer;
	size_t	head;
	size_t	tail;
	size_t	max;
	int		full:1;
};

typedef struct s_cbuf	t_cbuf;
typedef t_cbuf *		t_cbuf_handle;

t_cbuf_handle	cbuf_init(uint8_t *buffer, size_t size);
void			cbuf_free(t_cbuf_handle cbuf);
void			cbuf_reset(t_cbuf_handle cbuf);
void			cbuf_put(t_cbuf_handle cbuf, uint8_t data);
int				cbuf_put2(t_cbuf_handle cbuf, uint8_t data);
int				cbuf_get(t_cbuf_handle cbuf, uint8_t *data);
int				cbuf_empty(t_cbuf_handle cbuf);
int				cbuf_full(t_cbuf_handle cbuf);
size_t			cbuf_capacity(t_cbuf_handle cbuf);
size_t			cbuf_size(t_cbuf_handle cbuf);

#endif
