/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cbuf_full.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oyagci <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/09/09 13:08:01 by oyagci            #+#    #+#             */
/*   Updated: 2019/09/09 13:08:03 by oyagci           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cbuf.h"

int	cbuf_full(t_cbuf_handle cbuf)
{
	return (cbuf->full);
}
