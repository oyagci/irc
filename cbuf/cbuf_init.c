/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cbuf_init.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oyagci <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/09/09 13:08:01 by oyagci            #+#    #+#             */
/*   Updated: 2019/09/09 13:08:03 by oyagci           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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
