/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memmove.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abouhou <abouhou@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/22 16:25:47 by abouhou           #+#    #+#             */
/*   Updated: 2026/11/01 01:10:42 by abouhou          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memmove(void *dest, const void *src, size_t n)
{
	size_t				i;
	unsigned char		*dptr;
	const unsigned char	*sptr;

	dptr = dest;
	sptr = src;
	if (dest == src || n == 0)
		return (dest);
	if (dptr < sptr)
		ft_memcpy(dptr, sptr, n);
	else
	{
		i = n;
		while (i > 0)
		{
			i--;
			dptr[i] = sptr[i];
		}
	}
	return (dest);
}
