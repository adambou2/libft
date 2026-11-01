/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memcmp.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abouhou <abouhou@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/22 16:27:33 by abouhou           #+#    #+#             */
/*   Updated: 2026/10/30 21:35:13 by abouhou          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_memcmp(const void *s1, const void *s2, size_t n)
{
	size_t				i;
	const unsigned char	*sp1;
	const unsigned char	*sp2;

	i = 0;
	sp1 = (const unsigned char *)s1;
	sp2 = (const unsigned char *)s2;
	while (i < n)
	{
		if (sp1[i] - sp2[i] < 0)
			return (-1);
		if (sp1[i] - sp2[i] > 0)
			return (1);
		i++;
	}
	return (0);
}
