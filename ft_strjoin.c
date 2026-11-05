/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strjoin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abouhou <abouhou@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/25 15:59:14 by abouhou           #+#    #+#             */
/*   Updated: 2026/11/04 21:52:09 by abouhou          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strjoin(char const *s1, char const *s2)
{
	char	*str;
	size_t	total;

	if (!s1)
		return (ft_strdup(s2));
	if (!s2)
		return (ft_strdup(s1));
	if (!s1 && !s2)
		return (NULL);
	total = ft_strlen(s1) + ft_strlen(s2) + 1;
	str = ft_calloc(total, sizeof (char));
	if (!str)
		return (NULL);
	ft_strlcat(str, s1, total);
	ft_strlcat(str, s2, total);
	return (str);
}
