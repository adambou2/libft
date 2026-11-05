/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abouhou <abouhou@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/28 20:54:03 by abouhou           #+#    #+#             */
/*   Updated: 2026/11/05 09:10:05 by abouhou          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	word_count(char const *str, char c)
{
	int	count;
	int	i;

	i = 0;
	count = 0;
	if (!str)
		return (0);
	while (str[i])
	{
		while (str[i] == c && str[i])
			i++;
		if (str[i])
			count++;
		while (str[i] != c && str[i])
			i++;
	}
	return (count);
}

static int	word_len(char const *str, char c)
{
	int	i;

	i = 0;
	while (str[i] && str[i] != c)
		i++;
	return (i);
}

static void	cleanup(char **str, int count)
{
	int	i;

	i = 0;
	while (i < count)
	{
		free(str[i]);
		i++;
	}
	free(str);
}

static char	**word_set(char const *s, char **word, char c)
{
	int	i;
	int	j;
	int	len;

	i = 0;
	j = 0;
	while (s[i])
	{
		while (s[i] == c && s[i])
			i++;
		if (s[i])
		{
			len = word_len(&s[i], c);
			word[j] = ft_substr(s, i, len);
			if (!word[j])
			{
				cleanup(word, j);
				return (NULL);
			}
			j++;
			i += len;
		}
	}
	word[j] = NULL;
	return (word);
}

char	**ft_split(char const *s, char c)
{
	char	**word;

	if (!s)
		return (NULL);
	word = malloc(sizeof(char *) * (word_count(s, c) + 1));
	if (!word)
		return (NULL);
	if (!word_set(s, word, c))
		return (NULL);
	return (word);
}
