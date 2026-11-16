/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memmove.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abouhou <abouhou@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/22 16:25:47 by abouhou           #+#    #+#             */
/*   Updated: 2026/11/06 19:54:27 by abouhou          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memmove(void *dest, const void *src, size_t n)
{
	size_t		i;
	char		*dptr;
	const char	*sptr;

	dptr = (char *)dest;
	sptr = (const char *)src;
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

