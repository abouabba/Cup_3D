/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_malloc.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abouabba <abouabba@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/15 14:33:11 by rlamlaik          #+#    #+#             */
/*   Updated: 2025/10/17 22:40:44 by abouabba         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub.h"

void	ft_free(t_malloc **head)
{
	t_malloc	*helper;

	while (*head)
	{
		helper = *head;
		*head = (*head)->next;
		free(helper->toalloc);
		helper->toalloc = NULL;
		free(helper);
		helper = NULL;
	}
	*head = NULL;
}

void	*ft_malloc(unsigned int size, int flag)
{
	static t_malloc	*head;
	t_malloc		*to_save;

	if (flag)
	{
		to_save = malloc(sizeof(t_malloc));
		if (!to_save)
		{
			ft_free(&head);
			exit(1);
		}
		to_save->toalloc = malloc(size);
		if (!to_save->toalloc)
		{
			free(to_save);
			ft_free(&head);
			exit(1);
		}
		to_save->next = head;
		head = to_save;
		return (to_save->toalloc);
	}
	else
		ft_free(&head);
	return (NULL);
}
