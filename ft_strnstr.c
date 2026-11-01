/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strnstr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abouhou <abouhou@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/30 21:18:53 by abouhou           #+#    #+#             */
/*   Updated: 2026/10/30 21:34:45 by abouhou          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strnstr(const char *big, const char *little, size_t len)
{
	size_t	i;
	size_t	j;

	i = 0;
	if (little[i] == '\0')
		return ((char *)big);
	while (i < len && big[i])
	{
		j = 0;
		while (j < len - i && little[j]
			&& big[i + j] && big[i + j] == little[j])
			j++;
		if (!little[j])
			return ((char *)&big[i]);
		else
			i++;
	}
	return (NULL);
}
