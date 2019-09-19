/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cbuf_size.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oyagci <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/09/09 13:08:01 by oyagci            #+#    #+#             */
/*   Updated: 2019/09/09 13:08:03 by oyagci           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cbuf.h"

size_t	cbuf_size(t_cbuf_handle cbuf)
{
	size_t	size;

	size = cbuf->max;
	if (!cbuf->full)
	{
		if (cbuf->head >= cbuf->tail)
			size = cbuf->head - cbuf->tail;
		else
			size = cbuf->max + cbuf->head - cbuf->tail;
	}
	return (size);
}
