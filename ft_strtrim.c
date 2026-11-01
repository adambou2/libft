/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtrim.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abouhou <abouhou@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/28 13:16:56 by abouhou           #+#    #+#             */
/*   Updated: 2026/10/30 21:35:21 by abouhou          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	setchr(char const s, char const *set)
{
	int	i;

	i = 0;
	while (set[i])
	{
		if (s == set[i])
			return (1);
		i++;
	}
	return (0);
}

char	*ft_strtrim(char const *s1, char const *set)
{
	char	*str;
	int		total;
	int		i;
	int		j;

	if (!s1 || !set)
		return (NULL);
	i = 0;
	j = 0;
	if (s1[0])
		j = ft_strlen(s1) - 1;
	while (s1[i] && setchr(s1[i], set))
		i++;
	while (i <= j && setchr(s1[j], set))
		j--;
	if (s1[i] == '\0')
		return (ft_strdup(""));
	total = j - i + 2;
	str = malloc(sizeof(char) * total);
	if (!str)
		return (NULL);
	ft_strlcpy(str, &s1[i], total);
	return (str);
}
