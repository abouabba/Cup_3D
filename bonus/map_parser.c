/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map_parser.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abouabba <abouabba@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/18 15:49:56 by abouabba          #+#    #+#             */
/*   Updated: 2025/10/18 15:51:46 by abouabba         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub.h"

void	save_map_line(t_game *game, char *line)
{
	t_line	*tmp;
	t_line	*new;
	int		len;

	len = ft_strlen(line);
	if (len > 0 && line[len - 1] == '\n')
		line[len - 1] = '\0';
	new = ft_malloc(sizeof(t_line), 1);
	if (!new)
		return ;
	new->line = ft_strdup(line);
	new->next = NULL;
	if (!game->map_lines)
		game->map_lines = new;
	else
	{
		tmp = game->map_lines;
		while (tmp->next)
			tmp = tmp->next;
		tmp->next = new;
	}
}
