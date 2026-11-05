/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstsize.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abouhou <abouhou@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/31 14:17:15 by abouhou           #+#    #+#             */
/*   Updated: 2026/11/04 16:16:09 by abouhou          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

unsigned int	ft_lstsize(t_list *lst)
{
	t_list			*current;
	unsigned int	i;

	i = 0;
	current = lst;
	if (!lst)
		return (0);
	while (current != NULL)
	{
		current = current->next;
		i++;
	}
	return (i);
}
