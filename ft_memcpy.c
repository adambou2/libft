/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memcpy.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abouhou <abouhou@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/22 14:48:39 by abouhou           #+#    #+#             */
/*   Updated: 2026/11/03 02:00:44 by abouhou          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memcpy(void *dest, const void *src, size_t n)
{
	size_t				i;
	unsigned char		*dptr;
	const unsigned char	*sptr;

	i = 0;
	dptr = (unsigned char *)dest;
	sptr = (const unsigned char *)src;
	while (i < n)
	{
		dptr[i] = sptr[i];
		i++;
	}
	return (dest);
}
